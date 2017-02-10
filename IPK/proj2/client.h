#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include "common.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "uriparser.h"
#include <fstream>

class Client
{
public:
    Client(const std::string& _uri);
    ~Client();
private:
    void resolve(const boost::system::error_code &err, Ip::tcp::resolver::iterator ei);
    void connect(const boost::system::error_code &err, Ip::tcp::resolver::iterator ei);
    void read_response(const boost::system::error_code& err, std::string &msg);
    void read_header(const boost::system::error_code& err);
    void read_content(const boost::system::error_code& err);
    void read_data(const boost::system::error_code& err);
    void read_chunk_data(const boost::system::error_code& err);
    void reconstruct_chunks();
    void log_recv();
    void log_response();
    boost::asio::io_service io_service;
    Ip::tcp::resolver resolver;
    Ip::tcp::socket socket;
    boost::asio::streambuf request;
    boost::asio::streambuf response;
    std::istream response_stream;
    std::string status;
    std::vector<std::string> header;
    std::string data;
    bool chunkTransfer;
    std::ofstream fHeader;
    std::ofstream fPayload;
    std::ofstream in;
    std::ofstream out;
};

#endif /* !CLIENT_H */
