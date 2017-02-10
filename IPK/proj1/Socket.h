#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <netinet/in.h>
#include "Packet.h"

class Socket
{
public:
    Socket(std::string const& hostname,int port);
    Socket(int port);
    ~Socket();
    void Open();
    void Close();
    void Bind();
    void Listen();
    void Connect();
    static void Send(int socket,Packet &packet);
    void Send(Packet &packet);
    Packet Recv(unsigned int bytes);
    Packet Recv(int socket,unsigned int bytes);
    int getSocket() { return m_socket; }
private:
    void ProcessData(unsigned char flags,ClientPacket &dataPacket,int socket);
    std::string m_hostname;
    int m_port,m_socket;
    struct sockaddr_in m_sin;
};
#endif // !SOCKET_H
