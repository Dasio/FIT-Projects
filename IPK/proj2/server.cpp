#include "server.h"
#include "common.h"
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
boost::asio::io_service *iox;
std::ofstream in;
std::ofstream out;
Server::Server(int port, unsigned int _maxchunk, int _time) : io_service(),maxchunk(_maxchunk),time(_time), signals(io_service), acceptor(io_service), connection_manager(), socket(io_service)
{
    iox = &io_service;
    signals.add(SIGINT);
    signals.add(SIGTERM);
    await_stop();
    Ip::tcp::resolver resolver(io_service);
    Ip::tcp::endpoint endpoint = *resolver.resolve({"localhost",std::to_string(port)});
    acceptor.open(endpoint.protocol());
    acceptor.set_option(Ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();
    accept();
    std::string date = formatDateTime(boost::posix_time::second_clock::local_time(),"%Y-%m-%d:%H:%M:%S");
    in.open("ipkHttpServer-" + date + ".in.log", std::ofstream::out);
    out.open("ipkHttpServer-" + date + ".out.log", std::ofstream::out);
}
Server::~Server()
{
    in.close();
    out.close();
}
void Server::run()
{
    io_service.run();
}

void Server::accept()
{
    acceptor.async_accept(socket
        ,[this](boost::system::error_code ec)
        {
            if(!acceptor.is_open())
                return;
            if(!ec)
            {
                connection_manager.start(std::make_shared<Connection>(std::move(socket),connection_manager,maxchunk,time));
            }
            accept();
        });
}
void Server::await_stop()
{
    // Waiting asynchronosuly for signal
    signals.async_wait(
        [this](boost::system::error_code,int)
        {
            acceptor.close();
            connection_manager.stop_all();
            io_service.stop();
        });
}
