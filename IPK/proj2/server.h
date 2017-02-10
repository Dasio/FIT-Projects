#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <fstream>
#include "connection_manager.h"
#include "common.h"

class Server
{
public:
    Server(int port, unsigned int _maxchunk, int time);
    ~Server();
    void run();
    boost::asio::io_service io_service;
private:
    void accept();
    void await_stop();
    unsigned int maxchunk;
    int time;
    boost::asio::signal_set signals;
    Ip::tcp::acceptor acceptor;
    ConnectionManager connection_manager;
    Ip::tcp::socket socket;
};
#endif /* !SERVER_H */
