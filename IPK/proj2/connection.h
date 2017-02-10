#ifndef CONNECTION_H
#define CONNECTION_H

#include <array>
#include <memory>
#include <random>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include "common.h"
#include "reply.h"

extern boost::asio::io_service *iox;
class ConnectionManager;

class Connection: public std::enable_shared_from_this<Connection>
{
public:
    Connection(Ip::tcp::socket socketP,ConnectionManager& manager,unsigned int _maxchunk,int _time);
    void start();
    void stop();
private:
    static const boost::posix_time::time_duration timeout;

    void read();
    void write(bool chunk = false);
    void handle_timeout(const boost::system::error_code& ec);
    void handle_chunk_timer(const boost::system::error_code& ec);
    unsigned int send_chunk();
    Ip::tcp::socket socket;
    ConnectionManager& connection_manager;
    bool keepAlive;
    // For keep alive timeout
    boost::asio::deadline_timer tKeepAlive;
    // Sending chunks in intervals
    boost::asio::deadline_timer tChunk;
    std::array<char, 8192> _buffer;
    std::string buffer;
    reply _reply;
    unsigned int maxchunk;
    unsigned int chunkTime;
    // Random number generator
    std::uniform_int_distribution<unsigned int> distr;
    std::mt19937 eng;

};
typedef std::shared_ptr<Connection> connection_ptr;
#endif
