#include "client.h"
#include <cstring>
#include <sstream>
#include <iostream>

Client::Client(const std::string& _uri) : io_service(), resolver(io_service), socket(io_service), response_stream(&response), chunkTransfer(false)
{
    //o_service.run();
    Uri uri(_uri);
    std::ostream request_stream(&request);
    request_stream << "GET " << uri.path << " HTTP/1.1\r\n";
    request_stream << "Host: " << uri.server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: Keep-Alive\r\n\r\n";
    Ip::tcp::resolver::query query(uri.server, uri.port);
    resolver.async_resolve(query,boost::bind(&Client::resolve, this, boost::asio::placeholders::error,boost::asio::placeholders::iterator));
    //Logging
    std::string date = formatDateTime(boost::posix_time::second_clock::local_time(),"%Y-%m-%d:%H:%M:%S");
    in.open("ipkHttpClient-" + date + ".in.log", std::ofstream::out);
    out.open("ipkHttpClient-" + date + ".out.log", std::ofstream::out);
    io_service.run();
}
Client::~Client()
{
    in.close();
    out.close();
}
void Client::resolve(const boost::system::error_code &err, Ip::tcp::resolver::iterator ei)
{
    if(!err)
    {
        Ip::tcp::endpoint endpoint = *ei;
        socket.async_connect(endpoint,
            boost::bind(&Client::connect, this, boost::asio::placeholders::error, ++ei));
    }
}
void Client::connect(const boost::system::error_code &err, Ip::tcp::resolver::iterator ei)
{
    if(!err)
    {
        // Logging
        boost::asio::streambuf::const_buffers_type bufs = request.data();
        std::string msg(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size());
        // Send request
        boost::asio::async_write(socket,request,
            boost::bind(&Client::read_response, this, boost::asio::placeholders::error, msg));
    }
    else if (ei != Ip::tcp::resolver::iterator())
    {
        // Connection failed
        socket.close();
        Ip::tcp::endpoint ep = *ei;
        socket.async_connect(ep,
            boost::bind(&Client::connect, this, boost::asio::placeholders::error, ++ei));
    }
}
void Client::read_response(const boost::system::error_code& err, std::string& msg)
{
    if(!err)
    {
        out << msg;
        boost::asio::async_read_until(socket,response,"\r\n",
            boost::bind(&Client::read_header, this, boost::asio::placeholders::error));
    }
}
void Client::read_header(const boost::system::error_code& err)
{
    if(!err)
    {
        std::string http_version;
        response_stream >> http_version;
        unsigned int sc;
        response_stream >> sc;
        // Read from buffer remaining data
        status += http_version + " " + std::to_string(sc);
        std::getline(response_stream, http_version);
        status += http_version;
        in << status;
        // Stop reading
        if (sc != 200)
        {
            log_response();
            return;
        }
        boost::asio::async_read_until(socket,response,"\r\n\r\n",
            boost::bind(&Client::read_content,this, boost::asio::placeholders::error));
    }
}
void Client::read_content(const boost::system::error_code& err)
{
    if(err)
        return;
    log_recv();
    std::string buffer;
    while(true)
    {
        std::getline(response_stream,buffer);
        if(buffer == "\r")
            break;
        if(!chunkTransfer && buffer == "Transfer-Encoding: chunked\r")
            chunkTransfer = true;
        header.push_back(buffer);
    }
    std::ostringstream ss;
    ss << &response;
    data = ss.str();
    std::size_t found = 0;
    if(!chunkTransfer)
    {
        unsigned int c_length = 0;
        std::string con_len("Content-Length: ");
        for(auto h: header)
        {
            found = h.find(con_len);
            if(found != std::string::npos)
            {
                // Cast to int
                std::istringstream(h.substr(con_len.length())) >> c_length;
                break;
            }
        }
        // Content length was not found
        if(found == std::string::npos)
            return;
        // Everything was already read
        if(data.length() == c_length)
        {
            log_response();
            return;
        }
        // Read remaining data
        boost::asio::async_read(socket,response,boost::asio::transfer_at_least(c_length-data.length()),
            boost::bind(&Client::read_data,this,boost::asio::placeholders::error));
    }
    // Chunk encoding
    else
    {
        // Check if was already everything read
        if(data.find("0\r\n\r\n") != std::string::npos)
        {
            reconstruct_chunks();
        }
        // Read remaining data
        else
             boost::asio::async_read_until(socket,response,"0\r\n\r\n",
            boost::bind(&Client::read_chunk_data,this, boost::asio::placeholders::error));

    }
}
void Client::read_data(const boost::system::error_code& err)
{
    if(err)
        return;
    log_recv();
    std::ostringstream ss;
    ss << &response;
    data.append(ss.str());
    log_response();
}
void Client::read_chunk_data(const boost::system::error_code& err)
{
    if(err)
        return;
    log_recv();
    std::ostringstream ss;
    ss << &response;
    data.append(ss.str());
    reconstruct_chunks();
}
void Client::reconstruct_chunks()
{
    std::string object;
    std::istringstream iss(data);
    std::stringstream ss;
    std::string line;
    unsigned int count;
    while(std::getline(iss,line))
    {
        count = std::stoul(line, nullptr, 16);
        if(count == 0)
            break;
        // Not effective, nevermind :)
        char *buffer = new char[count];
        iss.read(buffer,count);
        object.append(buffer,count);
        iss.read(buffer,2); // Read \r\n
        delete[] buffer;
    }
    data = object;
    log_response();
}
void Client::log_recv()
{
    boost::asio::streambuf::const_buffers_type bufs = response.data();
    std::string msg(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
    in << msg;
}
void Client::log_response()
{
    std::string date = formatDateTime(boost::posix_time::second_clock::local_time(),"%Y-%m-%d:%H:%M:%S");
    fHeader.open("ipkResp-" + date + ".header", std::ofstream::out);
    fPayload.open("ipkResp-" + date + ".payload", std::ofstream::out);
    fHeader << status;
    for(auto h: header)
        fHeader << h;
    fPayload << data;
    fHeader.close();
    fPayload.close();
}
void showHelp()
{
    std::cout << "./ipkhttpclient [-h] URI" << std::endl;
}
int main(int argc, char**argv)
{
    if(argc < 2)
        return 1;
    if(!strcmp(argv[1],"-h"))
    {
        showHelp();
        return 0;
    }
    Client client(argv[1]);
    return 0;
}
