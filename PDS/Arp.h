#ifndef ARP_H

#include <stdint.h>
#include <vector>
#include "Packet.h"

class Arp : public Packet
{
public:
	static const uint8_t mac_len = 6;
	static const uint8_t ipv4_len = 4;
	struct arpHeader
	{
		uint16_t hw_type;
		uint16_t protocol_type;
		uint8_t hw_address_len;
		uint8_t protocol_address_len;
		uint16_t op_code;
		uint8_t src_hw[mac_len];
		uint8_t src_protocol[ipv4_len];
		uint8_t dst_hw[mac_len];
		uint8_t dst_protocol[ipv4_len];
	};

	Arp();
	~Arp();
	/**
	 * @brief Construct ARP Request packet
	 * 
	 * @param src_ip Source IPv4 address
	 * @param src_mac Source MAC address
	 * @param dst_ip Destination IPv4 address
	 */
	void request(uint8_t *src_ip, uint8_t *src_mac, uint8_t *dst_ip);

	/**
	 * @brief Construct ARP Reply packet
	 * 
	 * @param src_ip Source IPv4 address
	 * @param src_mac Source MAC address
	 * @param dst_ip Destination IPv4 address
	 * @param dst_mac Destination MAC address
	 */
	void reply(uint8_t *src_ip, uint8_t *src_mac, uint8_t *dst_ip, uint8_t *dst_mac);
	
	/**
	 * @brief Rewrite destinatip ip adress in ARP header
	 * 
	 * @param ip Destination IPv4 address
	 */
	void setDstIP(uint8_t *ip);
	void modifySrcMac(uint8_t *mac);
private:
	void fillBuffer();
	arpHeader header;
	
};

#endif
