#ifndef PACKET_H
#define PACKET_H

#include "common.h"
#include <vector>
#include <string>

class Packet
{
public:
	Packet();

	/**
	 * @brief Get packet buffer
	 * @return Raw data in packet
	 */
	const std::vector<uint8_t> &getBuffer() { return buffer; }

	/**
	 * @brief Rewrite packet buffer
	 * 
	 * @param data Pointer to data
	 * @param len Length of data
	 */
	void modify(uint8_t *data, uint16_t len);

	/**
	 * @brief Modify ethernet destination MAC address
	 * 
	 * @param mac MAC Address in integer form
	 */
	void setDstMac(uint64_t mac);

	/**
	 * @brief Modify ethernet source MAC address
	 * 
	 * @param mac MAC address (array of 6x uint8_t)
	 */
	void setSrcMac(uint8_t *mac);

	/**
	 * @brief Get type of protocol
	 * @return 1 in case of IPv4, 2 in case of IPv6, otherwise 0
	 */
	uint8_t getIPProtocol();

	/**
	 * @brief Get ethernet source ip
	 * @return IPv4 in string form
	 */
	std::string getSrcIP();

	/**
	 * @brief Get destination MAC address
	 * @return MAC address in integer form
	 */
	uint64_t getDstMac();

	/**
	 * @brief Source MAC address
	 * @return Pointer to buffer where src mac begins
	 */
	uint8_t *getSrcMac();

	/**
	 * @brief Source IPv6 address
	 * @return Pointer to buffer where source IPv6 begins
	 */
	uint8_t *getSrcIPv6();

protected:
	std::vector<uint8_t> buffer;
};


#endif
