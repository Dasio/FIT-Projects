#include <iostream>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Socket.h"
#include "MyExc.h"
#include <bitset>
#include "Passwd.h"


Socket::Socket(std::string const& hostname,int port) : m_hostname(hostname),m_port(port)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int port) : m_port(port)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() { }

void Socket::Open()
{
    if(m_socket == -1)
        throw MyExc("Socket::Open: Failed to create an endpoint for communication");
    m_sin.sin_family = AF_INET;
    m_sin.sin_port = htons(m_port);
}
void Socket::Close()
{
    if(close(m_socket) == -1)
        throw MyExc("Socket::Close: Failed to close socket");
}

void Socket::Bind()
{
    m_sin.sin_addr.s_addr  = INADDR_ANY;
    if((bind(m_socket, (struct sockaddr*)&m_sin, sizeof(m_sin))) == -1)
        throw MyExc("Socket::Bind: Failed to  bind a name to a socket");

}
void Socket::Listen()
{
    if(listen(m_socket,10) == -1)
         throw MyExc("Socket::Listen: Failed to listen for connections on a socket");
    socklen_t sinlen = sizeof(m_sin);
    std::vector<std::thread> threads;
    while(1)
    {
        int accepted_socket = accept(m_socket,(struct sockaddr*)&m_sin,&sinlen);
        if(accepted_socket == -1)
            throw MyExc("Socket::Listen: Failed to accept a connection on a socket");
        // Create new thread
        threads.push_back(std::thread([=](){
            struct hostent *hp = gethostbyaddr(&m_sin.sin_addr, 4, AF_INET);
            if(hp == NULL)
                throw MyExc("Socket::Listen: Failed to get host by adress");
            std::cout << "From " << inet_ntoa(m_sin.sin_addr) << " (" << hp->h_name << ") " << ntohs(m_sin.sin_port ) << std::endl;

            ClientPacket flagsPacket(Recv(accepted_socket,4));
            if(flagsPacket.getOpcode() != Opcode::CMSG_FLAGS)
                throw MyExc("Socket::Listen: First socket have to be flag packet");
            ClientPacket dataPacket = Recv(accepted_socket,flagsPacket.getDataSize());

            if(dataPacket.getOpcode() != Opcode::CMSG_DATA)
                throw MyExc("Socket::Listen: Second socket have to be data packet");
            // Op code is now useless
            ProcessData(flagsPacket.getFlags() >> 1,dataPacket,accepted_socket);
            if(close(accepted_socket) == -1)
                throw MyExc("Socket::Listen: Failed to close accepted socket");
        }));


    }
}

void Socket::Connect()
{
    struct hostent * hp = gethostbyname(m_hostname.c_str());
    if(hp == NULL)
        throw MyExc("Socket::Connect: Failed to get host by name");
    memcpy(&m_sin.sin_addr, hp->h_addr, hp->h_length);
    if(connect(m_socket, (struct sockaddr*)&m_sin, sizeof(m_sin)) == -1)
        throw MyExc("Socket::Connect: Failed to initiate a connection on a socket");
}

void Socket::Send(Packet &packet)
{
    if(write(m_socket, packet.getBuffer(), packet.getSize()) == -1)
        throw MyExc("Socket::Send: Failed to send a packet");
}

void Socket::Send(int socket,Packet &packet)
{
    if(write(socket, packet.getBuffer(), packet.getSize()) == -1)
        throw MyExc("Socket::Send: Failed to send a packet");
}

Packet Socket::Recv(int socket,unsigned int bytes)
{
    unsigned char buffer[bytes];
    int recv_bytes = read(socket, buffer, bytes);
    if(recv_bytes == -1)
        throw MyExc("Socket::Recv: Failed to receive a packet");
    return Packet(buffer,bytes);
}

Packet Socket::Recv(unsigned int bytes)
{
    unsigned char buffer[bytes];
    int recv_bytes = read(m_socket, buffer, bytes);
    if(recv_bytes == -1)
        throw MyExc("Socket::Recv: Failed to receive a packet");
    return Packet(buffer,bytes);
}

void Socket::ProcessData(unsigned char flags,ClientPacket &dataPacket,int socket)
{
    Passwd passwd(flags);
    // UIDs
    if(flags&1)
    {
        passwd.loadData(dataPacket.getUIDs());
    }
    // Logins
    else
    {
       passwd.loadData(dataPacket.getLogins());
    }
    std::vector<std::string> records = passwd.getRecords();
    unsigned int size = 1;

    for(auto string: records)
    {
        if(string == NOT_FOUND_S)
            size++;
        else
            size += string.length() + 1;
    }
    ServerPacket sizePacket(Opcode::SMSG_SIZE);
    sizePacket << size;
    ServerPacket sDataPacket(Opcode::SMSG_DATA,size);
    Send(socket,sizePacket);
    //std::cout << "Size = " << sizePacket.getDataSize() << std::endl;
    for(auto string: records)
    {
        if(string == NOT_FOUND_S)
            sDataPacket << (unsigned char)NOT_FOUND;
        else
            sDataPacket << string;
    }
    Send(socket,sDataPacket);
}
