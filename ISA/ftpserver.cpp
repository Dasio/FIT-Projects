#include "ftpserver.h"
#include "ISAExc.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <csignal>


const char* FTPServer::responseCodes[] = { "220\r\n", "331\r\n", "530\r\n"};
sig_atomic_t FTPServer::stopFlag = 0;
FTPServer::FTPServer(std::string const& address, unsigned int port, unsigned int maxClients, std::string logFile)
    : _port{port}, _maxClients{maxClients},_connections{0}, _address{address}
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    // IPv4 or IPv6
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // Server
    hints.ai_flags = AI_PASSIVE;
    // FTP is using TCP protocol
    hints.ai_protocol = IPPROTO_TCP;
    std::string p = std::to_string(port);
    int s = getaddrinfo(address.c_str(),p.c_str(),&hints,&results);
    if(s != 0)
    {
        std::cerr << gai_strerror(s);
        throw ISAExc("FTPServer: getaddrinfo");
    }
    // Iterate results from getaddrinfo and use first available
    for(rp = results; rp != nullptr; rp = rp->ai_next)
    {
        _socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(_socket == -1)
            continue;
        // Success
        if(bind(_socket, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(_socket);
    }
    // Binding failed
    if (rp == nullptr)
        throw ISAExc("Server: Failed to bind address");

    // Ignore error on write to closed socket, maybe use rather select() ?
    std::signal(SIGPIPE,SIG_IGN);
    std::signal(SIGINT,signalHandler);
    std::signal(SIGQUIT,signalHandler);

    // Open file for logging
    _logFile.open(logFile,std::fstream::out | std::fstream::app);

    // Start listening
    if(listen(_socket,_maxClients) == -1)
        throw ISAExc("Server: Failed to listen for connections on a socket");

}

FTPServer::~FTPServer()
{
    freeaddrinfo(results);
    close(_socket);
    _logFile.close();
}
void FTPServer::signalHandler(int signal)
{
    (void)signal;
    stopFlag = 1;
}
void FTPServer::run()
{
    socklen_t sinlen = sizeof(_sin);
    std::vector<std::thread> threads;


    while(!stopFlag)
    {
        fd_set set;
        struct timeval timeout;
        int rv;
        FD_ZERO(&set); /* clear the set */
        FD_SET(_socket, &set); /* add our file descriptor to the set */

        timeout.tv_sec = 20;
        timeout.tv_usec = 0;
        rv = select(_socket + 1, &set, NULL, NULL, &timeout);
        if(rv == -1)
            // Probably SIGINT
            break;
        else if(rv == 0)
        {
            //timeout
            if(stopFlag)
                break;
            continue;
        }
        int accepted_socket = accept(_socket, (struct sockaddr*)&_sin, &sinlen);
        if(accepted_socket == -1)
            break;
        // Check if we can accept this client
        if(incConnections())
        {
            closeConnection(accepted_socket);
            continue;
        }
        threads.push_back(std::thread([=](){
            //std::signal(SIGINT,SIG_IGN);
            char address[NI_MAXHOST];
            // Data required for each connection
            Data data;
            //Extract ip address from socket address
            getnameinfo((struct sockaddr*) &_sin, sizeof(_sin), address, sizeof(address), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
            data.address = std::string(address);
            data.state = READY_NEW_USER;
            data.bufferSize = 0;
            data.buffer.resize(defaultSize);

            startCommunication(accepted_socket,data);

            closeConnection(accepted_socket);
        }));
    }
    // Wait for all connection to finish
    for(auto &t: threads)
    {
        t.join();
    }
}


void FTPServer::startCommunication(int asocket, Data &data)
{
    while(!stopFlag && data.state != UNKNOWN)
    {
        sendResponse(asocket,data.state);
        // We already send send invalid login, we can close communication
        if(data.state == INVALID_LOGIN)
            break;
        // Timed out, try again
        int r;
        while(!stopFlag && (r = receive(asocket,data)) != 0);
        // r=2, recv() error
        if(stopFlag || r == 2)
            break;
        parseCommand(data);
    }
    log(data);
}
int FTPServer::receive(int asocket,Data &data)
{
    int bytes = 0;
    data.bufferSize = 0;
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    fd_set readFD;
    FD_ZERO(&readFD);
    FD_SET(asocket,&readFD);

    // Check if we can read data from socket
    if(select(asocket + 1, &readFD, NULL, NULL, &timeout) == -1)
        return 1;
    int iBytes;
    ioctl(asocket, FIONREAD, &iBytes);
    if(iBytes <=0)
        return 1;


    while (!stopFlag && (bytes = recv(asocket,&data.buffer[data.bufferSize], data.buffer.capacity() - data.bufferSize, 0)) > 0)
    {
        // Resize buffer if needed
        if(data.buffer.capacity() <= data.bufferSize + (unsigned)bytes)
            data.buffer.resize(data.buffer.capacity() * 2);
        data.bufferSize += bytes;
        // Check if FTP commaind is in buffer
        if(isReadyToParse(data))
            return 0;
    }
    if (bytes == -1)
    {
        std::cerr << "Server:recv - recv error" << std::endl;
        return 2;
    }
    return 0;

}
bool FTPServer::isReadyToParse(Data &data)
{
    return (data.buffer[data.bufferSize - 2] == '\r' && data.buffer[data.bufferSize - 1] == '\n');
}
void FTPServer::sendResponse(int asocket, States state)
{
    // 3digits + \r\n
    send(asocket, responseCodes[state], 5, 0);
}
void FTPServer::parseCommand(Data &data)
{
    char *end = strstr(data.buffer.data(), "\r\n");
    if(end == nullptr)
        return;
    if(!strncmp(data.buffer.data(), "USER ", 5))
    {
        data.state = CHALLENGE_PASSWORD;
        data.login.erase();
        // Save login
        for(int i=5;data.buffer.data() + i != end;i++)
            data.login.push_back(data.buffer.data()[i]);
    }
    else if(!strncmp(data.buffer.data(), "PASS ", 5))
    {
        data.state = INVALID_LOGIN;
        // Save password
        data.password.erase();
        for(int i=5;data.buffer.data() + i != end;i++)
            data.password.push_back(data.buffer.data()[i]);
        saveTime(data);

    }
    else
    {
        std::cerr << "unk command" << std::endl;
        data.state = UNKNOWN;
    }

}
void FTPServer::saveTime(Data &data)
{
    // Get actual time
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);
    data.date_time = std::string(buffer);
}
void FTPServer::closeConnection(int asocket)
{
    close(asocket);
    std::lock_guard<std::mutex> guard(mConnections);
    _connections--;
}

bool FTPServer::incConnections()
{
    std::lock_guard<std::mutex> guard(mConnections);
    return ++_connections > _maxClients;
}

void FTPServer::log(Data &data)
{
    if(data.date_time.empty())
        saveTime(data);
    // Lock mutex
    std::lock_guard<std::mutex> guard(mLog);
    _logFile << "FTP " << data.date_time << " " << data.address << " " << data.login << " " << data.password << std::endl;
}
