#ifndef SOCKET_H
#define SOCKET_H

#include <utility>
#include "common.h"
#include <string>
#include <linux/if_packet.h>
#include "Arp.h"

class Socket
{
public:
	Socket(int protocol);
	/**
	 * @brief Load required data about specified interface
	 * 
	 * @param interface String name of interface
	 */
	void loadInterface(std::string& interface);

	/**
	 * @brief Return lower na upper IPv4 address of subnet
	 * @return Pair(lower,upper)
	 */
	std::pair<uint32_t, uint32_t> getRange();
	
	/**
	 * @brief Send raw packet through socket
	 * 
	 * @param packet Raw packet
	 * @return 0 in case of failure(timeout)
	 */
	int send(Packet &packet);

	/**
	 * @brief Receive ARP packet
	 * @return Pointer to buffer where ARP header begins
	 */
	Arp::arpHeader* receive();

	/**
	 * @brief Receive packet
	 * 
	 * @param packet Pointer where packet will be stored
	 * @return Size
	 */
	uint16_t recvPacket(Packet *packet);
	uint8_t src_mac[6];
	IPv4 src_ip;
	IPv4 netmask;
	std::string ipv6;
private:
	void getIPv6(std::string &interface);
	int sd;
	struct sockaddr_ll device;
	std::vector<uint8_t> buffer;
};

#endif
