#include "sshserver.h"
#include "ISAExc.h"
#include <thread>
#include <vector>
#include <iostream>
#include <csignal>
#include <sys/ioctl.h>

sig_atomic_t SSHServer::stopFlag = 0;
SSHServer::SSHServer(std::string const& address, unsigned int port, unsigned int maxClients,
    unsigned int maxAttempts, const char *rsaFile, std::string logFile) :
    _port{port}, _maxClients{maxClients}, _maxAttempts{maxAttempts}, _connections{0}, _bind{ssh_bind_new()}
{
    // Set options, address,port and rsa key file
    ssh_bind_options_set(_bind,SSH_BIND_OPTIONS_BINDADDR,address.c_str());
    ssh_bind_options_set(_bind,SSH_BIND_OPTIONS_BINDPORT, &_port);
    ssh_bind_options_set(_bind,SSH_BIND_OPTIONS_RSAKEY, rsaFile);

    _logFile.open(logFile,std::fstream::out | std::fstream::app);
    std::signal(SIGINT,signalHandler);
    std::signal(SIGQUIT,signalHandler);
    if(ssh_bind_listen(_bind) < 0)
    {
        std::cerr << ssh_get_error(_bind) << std::endl;
        throw ISAExc("SSHServer: ssh_bind_listen failed listening to socket: ");
    }

}
SSHServer::~SSHServer()
{
    ssh_bind_free(_bind);
    ssh_finalize();
    _logFile.close();
}

void SSHServer::signalHandler(int signal)
{
    (void)signal;
    stopFlag = 1;
}

void SSHServer::run()
{
    ssh_session session;
    std::vector<std::thread> threads;
    int fd = ssh_bind_get_fd(_bind);
    while(!stopFlag)
    {
        fd_set set;
        struct timeval timeout;
        int rv;
        FD_ZERO(&set); /* clear the set */
        FD_SET(fd, &set); /* add our file descriptor to the set */

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        rv = select(fd + 1, &set, NULL, NULL, &timeout);
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
        session = ssh_new();
        if(ssh_bind_accept(_bind,session) != SSH_OK)
        {
            ssh_free(session);
            std::cerr << "SSHServer::run Failed to accept connection and init session" << std::endl;
            continue;
        }
        if(incConnections())
        {
            ssh_disconnect(session);
            ssh_free(session);
            decConnections();
            continue;
        }
        threads.push_back(std::thread([&this,session](){
            Data data;
            data.session = session;
            // Get address of client
            struct sockaddr_storage _sin;
            socklen_t peeraddrlen = sizeof(_sin);
            char address[NI_MAXHOST];
            // Get address of client to sockaddr _sin
            getpeername(ssh_get_fd(data.session),(struct sockaddr*)& _sin, &peeraddrlen);
            // Get addres o client from sockaddr to readeable format
            getnameinfo((struct sockaddr*) &_sin, sizeof(_sin), address, sizeof(address), NULL, 0, NI_NUMERICHOST | NI_NUMERICSERV);
            data.address = std::string(address);

            if(ssh_handle_key_exchange(data.session) == SSH_OK)
            {
                auth(data);
                // If log wasnt called even once, log at least time and ip without login
                if(data.date_time.empty())
                    log(data);
            }
            else
            {
                std::cerr << "SSHServer::run Failed to ssh_handle_key_exchange" << std::endl;
            }
            decConnections();
            ssh_free(session);

        }));
    }
    for(auto &t: threads)
    {
        t.join();
    }


}

void SSHServer::auth(Data &data)
{
    unsigned int attempts = 0;
    while(!stopFlag && attempts != _maxAttempts)
    {
        ssh_message msg = ssh_message_get(data.session);

        // Client disconnected
        if (msg == nullptr)
            return;
        int type = ssh_message_type(msg);
        switch (type)
        {
            case SSH_REQUEST_AUTH:
                switch(ssh_message_subtype(msg))
                {
                    case SSH_AUTH_METHOD_PASSWORD:
                        data.login = std::string(ssh_message_auth_user(msg));
                        data.password = std::string(ssh_message_auth_password(msg));
                        log(data);
                        attempts++;
                        if(attempts == _maxAttempts)
                            ssh_disconnect(data.session);
                        // Dont break, request password
                    default:
                        data.password.erase();
                        data.login = std::string(ssh_message_auth_user(msg));
                        // Request password
                        ssh_message_auth_set_methods(msg,SSH_AUTH_METHOD_PASSWORD);
                        ssh_message_reply_default(msg);
                }
                break;
            default:
                // First is SSH_REQUEST_SERVICE
                ssh_message_service_reply_success(msg);
                break;
        }
        ssh_message_free(msg);
    }
    if(attempts != _maxAttempts)
       ssh_disconnect(data.session);
}

void SSHServer::log(Data &data)
{
    // Get actual time
    time_t rawtime;
    struct tm * timeinfo;
    char datetime[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(datetime,80,"%Y-%m-%d %H:%M:%S",timeinfo);
    data.date_time = std::string(datetime);

    // Lock mutex
    std::lock_guard<std::mutex> guard(mLog);
    _logFile << "SSH " << data.date_time << " " <<  data.address << " " << data.login << " " << data.password << std::endl;
}

bool SSHServer::incConnections()
{
    std::lock_guard<std::mutex> guard(mConnections);
    return ++_connections > _maxClients;
}

void SSHServer::decConnections()
{
    std::lock_guard<std::mutex> guard(mConnections);
    _connections--;
}
