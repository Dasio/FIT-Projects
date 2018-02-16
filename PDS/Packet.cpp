#include "Packet.h"
#include <linux/if_ether.h> 
#include <netinet/ip6.h>
#include <iostream>

Packet::Packet()
{

}

void Packet::modify(uint8_t *data, uint16_t len)
{
	buffer.clear();
	buffer.insert(buffer.begin(), data, data + len);
}

uint8_t Packet::getIPProtocol()
{
	uint16_t type = (uint16_t)buffer[12] << 8 | buffer[13];
	if (type == ETH_P_IP)
		return 1;
	else if (type == ETH_P_IPV6)
		return 2;
	return 0;
}

void Packet::setDstMac(uint64_t mac)
{

	buffer[0] = mac >> 40;
	buffer[1] = mac >> 32;
	buffer[2] = mac >> 24;
	buffer[3] = mac >> 16;
	buffer[4] = mac >> 8;
	buffer[5] = mac;
}

void Packet::setSrcMac(uint8_t *mac)
{
	buffer[6] = mac[0];
	buffer[7] = mac[1];
	buffer[8] = mac[2];
	buffer[9] = mac[3];
	buffer[10] = mac[4];
	buffer[11] = mac[5];
}

std::string Packet::getSrcIP()
{
	uint8_t *ip = buffer.data() + 14 + 12;
	return getIP(ip);
}

uint64_t Packet::getDstMac()
{
	return MacBinToInt(buffer.data());
}

uint8_t *Packet::getSrcMac()
{
	return buffer.data() + 6;
}

uint8_t *Packet::getSrcIPv6()
{
	struct ip6_hdr *hdr = reinterpret_cast<struct ip6_hdr*>(buffer.data() + 14);
	return hdr->ip6_src.s6_addr;
}
