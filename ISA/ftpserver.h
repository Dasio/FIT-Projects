#ifndef FTPSERVER_H
#define FTPSERVER_H

#include <string>
#include <netinet/in.h>
#include <vector>
#include <mutex>
#include <fstream>
#include <signal.h>


 enum States
{
    READY_NEW_USER,
    CHALLENGE_PASSWORD,
    INVALID_LOGIN,
    UNKNOWN
};

/**
 * Contains data required for each connection
 */
struct Data
{
    int bufferSize;
    std::string password;
    States state;
    std::string login;
    std::vector<char> buffer;
    std::string address;
    std::string date_time;
};

class FTPServer
{
public:
    FTPServer(std::string const& address, unsigned int port, unsigned int maxClients, std::string logFile);
    ~FTPServer();
    void run();

private:

    static const char* responseCodes[];
    static const int defaultSize = 16;
    static sig_atomic_t stopFlag;

    /**
     * Receive data from socket
     * @param  asocket socket
     * @param  data    data
     * @return         1 if timeout occured, 2 - rev() error, otherwise 0
     */
    int receive(int asocket, Data &data);

    /**
     * Send FTP response code to socket based on actual state
     * @param asocket Client's socket
     * @param state   Actual state of client
     */
    void sendResponse(int asocket, States state);

    /**
     * Server start sending data to client and receiveing from client
     * @param asocket Client's socket
     * @param data    Reference to client's data
     */
    void startCommunication(int asocket,Data &data);

    /**
     * Received buffer is parsed to USER/LOGIN and actual state
     * @param data Reference to client's data
     */
    void parseCommand(Data &data);

    /**
     * Check if end of buffer contains \r\n
     * @param  data Reference to client's data
     * @return      true if contains \r\n otherwise false
     */
    bool isReadyToParse(Data &data);

    /**
     * Close client's socket and decrecrement counter of all connections(mutex used)
     * @param asocket Client's socket
     */
    void closeConnection(int asocket);

    /**
     * Increment counter of connections
     * @return true if actual connections counter is less than max allowed connections
     */
    bool incConnections();

    /**
     * Get actual time and save it to data.date_time
     */
    void saveTime(Data &data);

    /**
     * Log received client's data (mutex used)
     * @param data Reference to client's data
     */
    void log(Data &data);

    static void signalHandler(int signal);

    unsigned int _port, _maxClients, _connections;
    int _socket;
    std::string _address;
    struct sockaddr_storage _sin;
    struct addrinfo *results, *rp;
    std::ofstream _logFile;
    std::mutex mConnections;
    std::mutex mLog;



};
#endif // !FTPSERVER_H
