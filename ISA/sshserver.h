#ifndef SSHSERVER_H
#define SSHSERVER_H

#include <string>
#include <mutex>
#include <fstream>
#include <libssh/libssh.h>
#include <libssh/server.h>
#include <signal.h>




class SSHServer
{
public:
    SSHServer() = delete;
    SSHServer(std::string const& address, unsigned int port, unsigned int maxClients,
        unsigned int maxAttempts, const char *rsaFile, std::string logFile);
    ~SSHServer();
    void run();
private:
    static sig_atomic_t stopFlag;
    static void signalHandler(int signal);
    struct Data
    {
        ssh_session session;
        std::string login;
        std::string password;
        std::string address;
        std::string date_time;
    };
    /**
     * Starts getting messages from client and storing user/pass to data
     */
    void auth(Data &data);

    /**
     * Log collected data to logile
     */
    void log(Data &data);

    /**
     * thread-safe increasing connections
     * @return true if actual connections counter is less than max allowed connections
     */
    bool incConnections();
    /**
     * thread-safe decreasing connections
     */
    void decConnections();

    unsigned int _port, _maxClients, _maxAttempts, _connections;
    ssh_bind _bind;
    std::fstream _logFile;
    std::mutex mConnections;
    std::mutex mLog;
};

#endif //!SSHSERVER_H
