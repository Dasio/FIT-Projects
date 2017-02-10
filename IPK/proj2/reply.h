#ifndef REPLY_H
#define REPLY_H

#include <string>
#include <vector>
#include <boost/asio.hpp>

struct reply
{
    struct header
    {
        std::string name;
        std::string value;
    };

    enum status_type
    {
        ok = 200,
        bad_request = 400,
        not_found = 404
    } status;
    std::vector<header> headers;
    std::string content;
    std::vector<boost::asio::const_buffer> to_buffers();
    std::vector<boost::asio::const_buffer> chunk_to_buffer();
    static reply stock_reply(status_type status, bool chunk = false);
};



#endif
