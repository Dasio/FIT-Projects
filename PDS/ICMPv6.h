#ifndef ICMPV6_H
#define ICMPV6_H

#include "Packet.h"
#include <linux/if_ether.h> 
#include <netinet/ip6.h>      // struct ip6_hdr
#include <netinet/icmp6.h>    // struct icmp6_hdr and ICMP6_ECHO_REQUEST
#include <netinet/ether.h>


class ICMPv6 : public Packet
{
public:
	ICMPv6();
	/**
	 * @brief Construct ICMPv6 echo request packet
	 * 
	 * @param src_mac Source MAC address
	 * @param src_ip Source IPv6 address
	 */
	void echoRequest(uint8_t *src_mac, std::string &src_ip);

	/**
	 * @brief Construct ND Adverisement packet
	 * 
	 * @param src_mac Source MAC address
	 * @param src_ip Source IPv6 address
	 * @param dst_mac Destination MAC address
	 * @param dst_ip Destination IPv6 address
	 */
	void naAdvert(uint8_t *src_mac, const std::string &src_ip,  uint8_t *dst_mac, const std::string &dst_ip);
	void modifySrcMac(uint8_t *mac);
private:
	/**
	 * @brief Compute checksum for echo request
	 * @return Checksum for ICMPv6
	 */
	uint16_t checkSum();

	/**
	 * @brief Compute checksum for NDP packet
	 * @return Checksum
	 */
	uint16_t checkSumND();
	void fillEthHeader(const uint8_t *src_mac, const uint8_t *dst_mac);
	void fillIP6Header(const std::string &src_ip, const std::string &dst_ip);
	void fillICMPEchoHeader();
	void fillNDAHeader();
	void fillBuffer();
	std::vector<uint8_t> icmpBuffer;
	struct ether_header eh;
	struct ip6_hdr ip6_header;
	struct icmp6_hdr icmp6_header;
	struct nd_neighbor_advert na;
	struct nd_opt_hdr na_opt;
};

#endif
