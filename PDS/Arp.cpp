#include "Arp.h"
#include <arpa/inet.h>
#include <linux/if_ether.h> 
#include <iostream>
#include <string.h>

Arp::Arp()
{
	buffer.reserve(sizeof(header) + 14);
	// Ethernet
	header.hw_type = htons(1);
	// IPv4
	header.protocol_type = htons(0x0800);
	header.hw_address_len = mac_len;
	header.protocol_address_len = ipv4_len;
	header.op_code = htons(1);
}

void Arp::fillBuffer()
{
	buffer.clear();
	// Ethernet
	buffer.insert(buffer.end(), header.dst_hw, header.dst_hw + header.hw_address_len);
	buffer.insert(buffer.end(), header.src_hw, header.src_hw + header.hw_address_len);
	buffer.push_back(ETH_P_ARP / 256);
	buffer.push_back(ETH_P_ARP % 256);
	// ARP
	buffer.insert(buffer.end(), (uint8_t*)&header, ((uint8_t*)&header) + sizeof(header));
}
void Arp::request(uint8_t *src_ip, uint8_t *src_mac, uint8_t *dst_ip)
{
	header.op_code = htons(1);

	memcpy(header.src_protocol, src_ip, ipv4_len);
	memcpy(header.dst_protocol, dst_ip, ipv4_len);

	memcpy(header.src_hw, src_mac, mac_len);
	// Target is broadcast
	memset(header.dst_hw, 0xFF, mac_len);

	fillBuffer();
}

void Arp::reply(uint8_t *src_ip, uint8_t *src_mac, uint8_t *dst_ip, uint8_t *dst_mac)
{
	header.op_code = htons(2);
	memcpy(header.src_protocol, src_ip, ipv4_len);
	memcpy(header.dst_protocol, dst_ip, ipv4_len);
	memcpy(header.src_hw, src_mac, mac_len);
	memcpy(header.dst_hw, dst_mac, mac_len);
	fillBuffer();
}	

void Arp::setDstIP(uint8_t *ip)
{
	memcpy(header.dst_protocol, ip, ipv4_len);
	uint8_t *dst_ip = buffer.data() + 2*header.hw_address_len + 2 + 8 + 2 * mac_len + ipv4_len;
	memcpy(dst_ip, ip, ipv4_len);
}

void Arp::modifySrcMac(uint8_t *mac)
{
	memcpy(header.src_hw, mac, mac_len);
	fillBuffer();
}

Arp::~Arp()
{
}
