#include "Packet.h"
#include <cstring>
#include <bitset>
#include <iostream>
#include "Passwd.h"

Packet::Packet(Opcode op,unsigned int size) : m_size(size),m_writepos(0)
{
    m_buffer.resize(m_size);
    m_buffer[0] = op;
}

Packet::Packet(const unsigned char* buffer, unsigned int size) : m_size(size)
{
    m_buffer.resize(m_size);
    memcpy(&m_buffer[0], buffer, m_size);
}
void Packet::Init(Opcode op,unsigned int size)
{
    m_size = size;
    m_writepos = 0;
    m_buffer.resize(m_size);
    m_buffer[0] = op;
}


Packet& Packet::operator<<(const unsigned int data)
{
    memcpy(&m_buffer[1] + m_writepos, &data, sizeof(data));
    m_writepos += sizeof(data);
    return *this;
}
Packet& Packet::operator<<(const std::string &data)
{
    memcpy(&m_buffer[1] + m_writepos, data.data(), data.length() + 1);
    m_writepos += data.length() + 1;
    return *this;
}

Packet& Packet::operator<<(const unsigned char data)
{
    memcpy(&m_buffer[1] + m_writepos, &data, sizeof(data));
    m_writepos += sizeof(data);
    return *this;
}

ClientPacket::ClientPacket(Packet packet) : Packet(packet)
{

}

void ClientPacket::WriteFlagsData(unsigned char flags,unsigned int size)
{
    // Opcode is at first bit
    m_buffer[0] |= flags << 1;
    memcpy(&m_buffer[1], &size, 3);
}

std::vector<unsigned int> ClientPacket::getUIDs()
{
    std::vector<unsigned int> uids;
    unsigned int to = m_size / sizeof(unsigned int);
    for(unsigned int i=0;i<to;i++)
        uids.push_back(getUID(i));
    return uids;

}

std::vector<std::string> ClientPacket::getLogins()
{
    std::vector<std::string> logins;
    std::string buffer;
    for(unsigned int i=1; i<m_size; i++)
    {
        if(m_buffer[i] == '\0')
        {
            logins.push_back(buffer);
            buffer.erase();
        }
        else
            buffer.push_back(m_buffer[i]);
    }
    return logins;
}

ServerPacket::ServerPacket(Packet packet) : Packet(packet),m_readpos(1)
{

}

std::vector<std::string> ServerPacket::getRecords()
{
    std::vector<std::string> records;
    std::string buffer;
    for(unsigned int i=1; i<m_size; i++)
    {
        if(m_buffer[i] == NOT_FOUND)
        {
            records.push_back(NOT_FOUND_S);
        }
        else if(m_buffer[i] == '\0')
        {
            records.push_back(buffer);
            buffer.erase();
        }
        else
            buffer.push_back(m_buffer[i]);
    }
    return records;
}

std::vector<std::string> ServerPacket::getRecord()
{
    std::vector<std::string> record;
    std::string buffer;
    for(; m_readpos<m_size && m_buffer[m_readpos] != '\0'; m_readpos++)
    {
        //std::cout << m_buffer[m_readpos];
        if(m_buffer[m_readpos] == NOT_FOUND)
        {
            record.push_back(std::string(1,NOT_FOUND));
            break;
        }
        else if(m_buffer[m_readpos] == DELIMETER)
        {
            record.push_back(buffer);
            buffer.erase();
        }
        else
            buffer.push_back(m_buffer[m_readpos]);
    }
    m_readpos++;
    return record;
}

ServerPacket& ServerPacket::operator>>(std::vector<std::string> &record)
{
    std::string buffer;
    for(; m_readpos<m_size && m_buffer[m_readpos] != '\0'; m_readpos++)
    {
        if(m_buffer[m_readpos] == DELIMETER)
        {
            record.push_back(buffer);
            buffer.erase();
        }
        else
            buffer.push_back(m_buffer[m_readpos]);
    }
    m_readpos++;
    return *this;
}
