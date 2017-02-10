#include "common.h"
#include "connection.h"
#include "connection_manager.h"
#include "request_parser.h"
#include "reply.h"
#include <boost/bind.hpp>
#include <fstream>
#include "boost/filesystem.hpp"

const boost::posix_time::time_duration Connection::timeout = boost::posix_time::seconds(30);
extern std::ofstream in;
extern std::ofstream out;

std::string to_hex_string(unsigned int value)
{
  std::ostringstream stream;
  stream << std::hex << value;
  return stream.str();
}
Connection::Connection(Ip::tcp::socket socketP,ConnectionManager& manager,unsigned int _maxchunk,int _time) : socket(std::move(socketP)),connection_manager(manager),
        keepAlive(false),tKeepAlive(*iox),tChunk(*iox),maxchunk(_maxchunk),chunkTime(_time),distr(chunkTime,2*chunkTime)
    {
        std::random_device rd;
        std::mt19937 eng(rd());
        tKeepAlive.expires_from_now(timeout);
        tChunk.expires_from_now(boost::posix_time::milliseconds(distr(eng)));

    }
void Connection::start()
{
    read();
}

void Connection::stop()
{
    socket.close();
}

void Connection::read()
{
    auto self(shared_from_this());
    socket.async_read_some(boost::asio::buffer(_buffer),
        [this, self](boost::system::error_code ec, std::size_t size)
        {
            if(!ec)
            {
                std::string msg(_buffer.data(),size);
                in << msg;
                in.flush();
                RequestParser parser(_buffer);
                parser.parse();
                keepAlive = parser.keepAlive;
                // Sending in chunks
                if(maxchunk)
                {
                    bool genList = false;
                    bool index = false;
                    std::string path;
                    if(!url_decode(parser.path,path))
                    {
                        _reply = reply::stock_reply(reply::bad_request);
                        write();
                        return;
                    }
                    path = RequestParser::doc + path;
                    // If file is not found, send 404
                    if(!boost::filesystem::exists(path))
                    {
                        _reply = reply::stock_reply(reply::not_found);
                        write();
                        return;
                    }
                    _reply = reply::stock_reply(reply::ok,true);
                     if(keepAlive)
                        _reply.headers.push_back({"Connection","Keep-Alive"});
                    boost::filesystem::path p(path);
                    if(is_directory(p))
                    {
                        if(boost::filesystem::exists(path + "/index.html"))
                        {
                            path += "/index.html";
                            index = true;
                        }
                        else
                        {
                            buffer = RequestParser::gen_list(path);
                            genList = true;
                        }
                        _reply.headers[1].value = "text/html";
                    }

                    if(!genList && !index)
                        _reply.headers[1].value = RequestParser::ext2mime(p.extension().string());
                    // Extract extension of file, convert to mime and send header
                    write();
                    unsigned int size = maxchunk;
                    if(maxchunk > 512)
                        size = 512;
                    char buf[512];
                    std::ifstream is(path, std::ios::in | std::ios::binary);
                    while (!genList && is.read(buf, size).gcount() > 0)
                    {
                        buffer.append(buf,is.gcount());
                        // Start sending in intervals
                        if(chunkTime)
                        {
                            tChunk.async_wait(boost::bind(&Connection::handle_chunk_timer, this, boost::asio::placeholders::error));
                        }
                        // Sending without delay
                        else if(buffer.length() >= maxchunk)
                        {
                            send_chunk();
                        }
                    }
                    // Send files list
                    if(genList)
                    {
                        // Send in intervals
                        if(chunkTime)
                        {
                            tChunk.async_wait(boost::bind(&Connection::handle_chunk_timer, this, boost::asio::placeholders::error));
                        }
                        else
                        {
                            // Send immediately
                            while(!buffer.empty())
                            {
                                send_chunk();
                            }
                            // Send 0/r/n/r/n
                            send_chunk();
                        }


                    }

                    else if(!chunkTime)
                    {
                        // Send remaining data
                        if(!buffer.empty())
                            send_chunk();
                        // Send 0/r/n/r/n
                        send_chunk();
                    }

                }
                else
                {
                    _reply = parser.handle();
                    write();
                }

            }
            else if(ec != boost::asio::error::operation_aborted && !keepAlive)
            {
                connection_manager.stop(shared_from_this());
            }
        });
}
void Connection::write(bool chunk)
{
    auto func = _reply.to_buffers();
    if(chunk)
        func = _reply.chunk_to_buffer();
    auto self(shared_from_this());
    boost::asio::async_write(socket,func,
        [this, self, func, chunk](boost::system::error_code ec, std::size_t)
        {
            tKeepAlive.expires_from_now(timeout);
            if(!ec)
            {
                // Logging
                std::string msg;
                if(!chunk)
                {
                   for(auto x: func)
                    {
                        std::size_t size = boost::asio::buffer_size(x);
                        msg.append(boost::asio::buffer_cast<const char *>(x),size);
                    }
                    out << msg;
                }
                out.flush();
                boost::system::error_code ignored_ec;
                if(keepAlive)
                {
                    tKeepAlive.async_wait(boost::bind(&Connection::handle_timeout, this, boost::asio::placeholders::error));
                    read();
                }
                else
                    socket.shutdown(Ip::tcp::socket::shutdown_both, ignored_ec);
            }
            if(ec != boost::asio::error::operation_aborted && !keepAlive)
            {
                connection_manager.stop(shared_from_this());
            }
        });
}

void Connection::handle_timeout(const boost::system::error_code& ec)
{
    if(!ec)
    {
        if (tKeepAlive.expires_at() <= boost::asio::deadline_timer::traits_type::now())
        {
            // The deadline has passed. Do a graceful shutdown
            boost::system::error_code ignored_ec;
            socket.shutdown(Ip::tcp::socket::shutdown_both, ignored_ec);
            connection_manager.stop(shared_from_this());
        }
    }
}
void Connection::handle_chunk_timer(const boost::system::error_code& ec)
{
    if(!ec && tChunk.expires_at() <= boost::asio::deadline_timer::traits_type::now())
    {
        unsigned int send = send_chunk();
        // Generate random number from interval
        tChunk.expires_from_now(boost::posix_time::milliseconds(distr(eng)));
        // If is something in buffer, send next chunk
        if(send)
            tChunk.async_wait(boost::bind(&Connection::handle_chunk_timer, this, boost::asio::placeholders::error));

    }
}
unsigned Connection::send_chunk()
{
    _reply.content.clear();
    unsigned int size = maxchunk < buffer.length() ? maxchunk : buffer.length();
    _reply.content.append(to_hex_string(size));
    _reply.content.append("\r\n");
    _reply.content.append(buffer.substr(0,size));
    _reply.content.append("\r\n");
    write(true);
    out << _reply.content;
    //out.flush();
    buffer.erase(0,size);
    return size;
}
