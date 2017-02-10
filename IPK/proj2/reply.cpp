#include "reply.h"
namespace status
{
    const std::string ok = "HTTP/1.1 200 OK\r\n";
    const std::string bad_request = "HTTP/1.1 400 Bad Request\r\n";
    const std::string not_found = "HTTP/1.1 404 Not Found\r\n";
    const std::string internal_server_error = "HTTP/1.1 500 Internal Server Error\r\n";
    boost::asio::const_buffer to_buffer(reply::status_type status)
    {
        switch (status)
        {
            case reply::ok:
                return boost::asio::buffer(ok);
            case reply::bad_request:
                return boost::asio::buffer(bad_request);
            case reply::not_found:
                return boost::asio::buffer(not_found);
            default:
                return boost::asio::buffer(internal_server_error);
        }
    }
}
namespace reply_strings
{
    const char ok[] = "";
    const char bad_request[] =
      "<html>"
      "<head><title>Bad Request</title></head>"
      "<body><h1>400 Bad Request</h1></body>"
      "</html>";
    const char not_found[] =
      "<html>"
      "<head><title>Not Found</title></head>"
      "<body><h1>404 Not Found</h1></body>"
      "</html>";
    const char internal_server_error[] =
      "<html>"
      "<head><title>Internal Server Error</title></head>"
      "<body><h1>500 Internal Server Error</h1></body>"
      "</html>";
    std::string to_string(reply::status_type status)
    {
        switch (status)
        {
        case reply::ok:
            return ok;
        case reply::bad_request:
            return bad_request;
        case reply::not_found:
            return not_found;
        default:
            return internal_server_error;
        }
    }
}
const char value_sep[] = { ':', ' '};
const char crlf[] = {'\r','\n'};
std::vector<boost::asio::const_buffer> reply::to_buffers()
{
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(status::to_buffer(status)));
    for(std::size_t i = 0; i < headers.size(); ++i)
    {
        buffers.push_back(boost::asio::buffer(headers[i].name));
        buffers.push_back(boost::asio::buffer(value_sep));
        buffers.push_back(boost::asio::buffer(headers[i].value));
        buffers.push_back(boost::asio::buffer(crlf));
    }
    buffers.push_back(boost::asio::buffer(crlf));
    buffers.push_back(boost::asio::buffer(content));
    return buffers;
}
std::vector<boost::asio::const_buffer> reply::chunk_to_buffer()
{
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(content));
    return buffers;
}
reply reply::stock_reply(reply::status_type status,bool chunk)
{
    reply rep;
    rep.status = status;
    rep.content = reply_strings::to_string(status);
    rep.headers.resize(2);
    if(chunk)
    {
        rep.headers[0].name = "Transfer-Encoding";
        rep.headers[0].value = "chunked";
    }
    else
    {
        rep.headers[0].name = "Content-Length";
        rep.headers[0].value = std::to_string(rep.content.size());
    }
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = "text/html";
    return rep;
}
