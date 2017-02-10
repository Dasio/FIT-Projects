#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <string>

enum Opcode : unsigned char
{
    CMSG_FLAGS = 0,
    CMSG_DATA = 1,
    SMSG_SIZE = 0,
    SMSG_DATA = 1
};

class Packet
{
public:
    Packet() { };
    Packet(Opcode op,unsigned int size = 4);
    Packet(const unsigned char* buffer, unsigned int size = 4);
    void Init(Opcode op,unsigned int size = 4);
    void AppendFlags(unsigned char flags);

    unsigned int getSize() { return m_size; }
    const unsigned char* getBuffer() const { return &m_buffer[0]; }
    unsigned char getOpcode() { return m_buffer[0] & 1; }
    unsigned int getDataSize() { return (*((unsigned int*)&m_buffer[0])) >> 8; }


    Packet& operator<<(const unsigned int data);
    Packet& operator<<(const unsigned char data);
    Packet& operator<<(const std::string &data);


protected:
    unsigned int m_size,m_writepos;
    std::vector<unsigned char> m_buffer;
};

class ClientPacket : public Packet
{
    using Packet::Packet;
public:
    ClientPacket() = default;
    ClientPacket(Packet packet);

    unsigned char getFlags() { return m_buffer[0]; };
    unsigned int getUID(unsigned int i) { return *(((unsigned int*)&m_buffer[1]) + i); }
    void WriteFlagsData(unsigned char flags,unsigned int size);
    std::vector<unsigned int> getUIDs();
    std::vector<std::string> getLogins();


};
class ServerPacket : public Packet
{
    using Packet::Packet;
public:
    ServerPacket() = default;
    ServerPacket(Packet packet);
    std::vector<std::string> getRecords();
    std::vector<std::string> getRecord();

    ServerPacket& operator>>(std::vector<std::string> &record);
private:
    unsigned int m_readpos;
};
#endif //!PACKET_H
