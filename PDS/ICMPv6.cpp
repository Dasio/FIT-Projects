#include "ICMPv6.h"
#include <arpa/inet.h>
#include <netinet/ip.h> 
#include <stdexcept>
#include <iostream>

static const uint8_t icmp_hdrlen = 8;
static const uint8_t icmp_echo_datalen = 8;

// http://www.pdbuchan.com/rawsock/ping6_ll.c
uint16_t checksum (uint16_t *addr, int len)
{
  int count = len;
  register uint32_t sum = 0;
  uint16_t answer = 0;

  // Sum up 2-byte values until none or only one byte left.
  while (count > 1) {
    sum += *(addr++);
    count -= 2;
  }

  // Add left-over byte, if any.
  if (count > 0) {
    sum += *(uint8_t *) addr;
  }

  // Fold 32-bit sum into 16 bits; we lose information by doing this,
  // increasing the chances of a collision.
  // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }

  // Checksum is one's compliment of sum.
  answer = ~sum;

  return (answer);
}

ICMPv6::ICMPv6()
{
	buffer.reserve(IP_MAXPACKET);
	eh.ether_type = htons(ETH_P_IPV6);
}


void ICMPv6::echoRequest(uint8_t *src_mac, std::string &src_ip)
{
	static const uint8_t dst_mac[6] = { 0x33, 0x33, 0x00, 0x00, 0x00, 0x01 };
	static const std::string dst_ip("ff02::1");
	fillEthHeader(src_mac, dst_mac);
	fillIP6Header(src_ip, dst_ip);
	fillICMPEchoHeader();
	fillBuffer();
}

void ICMPv6::naAdvert(uint8_t *src_mac, const std::string &src_ip,  uint8_t *dst_mac, const std::string &dst_ip)
{
	fillEthHeader(src_mac, dst_mac);
	fillIP6Header(src_ip, dst_ip);
	ip6_header.ip6_plen = htons (32);

	fillNDAHeader();
	
	buffer.clear();
	// Insert Ether frame
	buffer.insert(buffer.end(), (uint8_t*)&eh, ((uint8_t*)&eh) + sizeof(eh));
	// Insert IP6 header
	buffer.insert(buffer.end(), (uint8_t*)&ip6_header, ((uint8_t*)&ip6_header) + sizeof(ip6_header));
	buffer.insert(buffer.end(), (uint8_t*)&na, ((uint8_t*)&na) + sizeof(na));
	buffer.insert(buffer.end(), (uint8_t*)&na_opt, ((uint8_t*)&na_opt) + sizeof(na_opt));
	buffer.insert(buffer.end(), src_mac, src_mac + 6);
}


void ICMPv6::fillEthHeader(const uint8_t *src_mac, const uint8_t *dst_mac)
{
	memcpy(eh.ether_dhost, dst_mac, 6);
	memcpy(eh.ether_shost, src_mac, 6);
}
void ICMPv6::fillIP6Header(const std::string &src_ip, const std::string &dst_ip)
{
	// IPv6 version (4 bits), Traffic class (8 bits), Flow label (20 bits)
  	ip6_header.ip6_flow = htonl ((6 << 28) | (0 << 20) | 0);
  	ip6_header.ip6_plen = htons (icmp_hdrlen + 8);
  	// Next header (8 bits): 58 for ICMP
  	ip6_header.ip6_nxt = IPPROTO_ICMPV6;
  	ip6_header.ip6_hops = 255;
  	int status;
  	status = inet_pton(AF_INET6, src_ip.data(), &(ip6_header.ip6_src));
  	if (status != 1)
  		throw std::runtime_error(std::string("ICMPv6::fillIP6Header() - load src_ip")
			.append(strerror(status)));

  	status = inet_pton(AF_INET6, dst_ip.data(), &(ip6_header.ip6_dst));
  	if (status != 1)
  		throw std::runtime_error(std::string("ICMPv6::fillIP6Header() - load dst_ip")
			.append(strerror(status)));
}

void ICMPv6::fillBuffer()
{
	buffer.clear();
	// Insert Ether frame
	buffer.insert(buffer.end(), (uint8_t*)&eh, ((uint8_t*)&eh) + sizeof(eh));
	// Insert IP6 header
	buffer.insert(buffer.end(), (uint8_t*)&ip6_header, ((uint8_t*)&ip6_header) + sizeof(ip6_header));
	// Insert ICMPv6 header
	buffer.insert(buffer.end(), (uint8_t*)&icmp6_header, ((uint8_t*)&icmp6_header) + sizeof(icmp6_header));
	// Insert ICMPv6 data
	buffer.insert(buffer.end(), icmpBuffer.begin(), icmpBuffer.end());

}

void ICMPv6::fillICMPEchoHeader()
{
	static const uint8_t data[icmp_echo_datalen] = {0xcd,0x25,0xcd,0x25,0xcd,0x25,0xcd,0x25};
	icmpBuffer.clear();
	icmpBuffer.insert(icmpBuffer.end(), data, data + icmp_echo_datalen);
	icmp6_header.icmp6_type = ICMP6_ECHO_REQUEST;
	icmp6_header.icmp6_code = 0;
	icmp6_header.icmp6_id = htons(42);
	icmp6_header.icmp6_seq = htons(0);
	icmp6_header.icmp6_cksum = 0;
	icmp6_header.icmp6_cksum = checkSum();
}

void ICMPv6::fillNDAHeader()
{
	na.nd_na_hdr.icmp6_type = ND_NEIGHBOR_ADVERT;
	na.nd_na_hdr.icmp6_code = 0;
	na.nd_na_hdr.icmp6_cksum = htons(0);
	na.nd_na_flags_reserved = htonl((1 << 29));
	na.nd_na_target = ip6_header.ip6_src;
	na_opt.nd_opt_type = 2;
	na_opt.nd_opt_len = 1;

	na.nd_na_hdr.icmp6_cksum = checkSumND();
}
uint16_t ICMPv6::checkSumND()
{
	std::vector<uint8_t> buff;
	buff.insert(buff.end(), (uint8_t*)&(ip6_header.ip6_src), ((uint8_t*)&(ip6_header.ip6_src)) + sizeof(ip6_header.ip6_src));
	buff.insert(buff.end(), (uint8_t*)&(ip6_header.ip6_dst), ((uint8_t*)&(ip6_header.ip6_dst)) + sizeof(ip6_header.ip6_dst));
	buff.push_back((sizeof(na) + 8) / 256);
	buff.push_back((sizeof(na) + 8) % 256);
	buff.push_back(0); buff.push_back(0); buff.push_back(0);
	buff.push_back(IPPROTO_ICMPV6);
	buff.insert(buff.end(), (uint8_t*)&(na), ((uint8_t*)&(na)) + sizeof(na));
	buff.insert(buff.end(), (uint8_t*)&(na_opt), ((uint8_t*)&(na_opt)) + sizeof(na_opt));
	buff.insert(buff.end(), eh.ether_shost, eh.ether_shost + 6);
	// Padding
	if(buff.size() % 2 == 1)
		buff.push_back(0);
	return checksum ((uint16_t*) buff.data(), buff.size());
}

uint16_t ICMPv6::checkSum()
{
	std::vector<uint8_t> buff;
	buff.insert(buff.end(), (uint8_t*)&(ip6_header.ip6_src), ((uint8_t*)&(ip6_header.ip6_src)) + sizeof(ip6_header.ip6_src));
	buff.insert(buff.end(), (uint8_t*)&(ip6_header.ip6_dst), ((uint8_t*)&(ip6_header.ip6_dst)) + sizeof(ip6_header.ip6_dst));
	buff.push_back((icmp_hdrlen + icmpBuffer.size()) / 256);
	buff.push_back((icmp_hdrlen + icmpBuffer.size()) % 256);
	buff.push_back(0); buff.push_back(0); buff.push_back(0);
	buff.insert(buff.end(), (uint8_t*)&(ip6_header.ip6_nxt), ((uint8_t*)&(ip6_header.ip6_nxt)) + sizeof(ip6_header.ip6_nxt));
	// Insert ICMPv6 header
	buff.insert(buff.end(), (uint8_t*)&icmp6_header, ((uint8_t*)&icmp6_header) + sizeof(icmp6_header));
	// Insert ICMPv6 data
	buff.insert(buff.end(), icmpBuffer.begin(), icmpBuffer.end());
	// Padding
	if(buff.size() % 2 == 1)
		buff.push_back(0);
	return checksum ((uint16_t*) buff.data(), buff.size());
}

void ICMPv6::modifySrcMac(uint8_t *mac)
{
	naAdvert(mac, IPv6ToString((uint8_t*)&(ip6_header.ip6_src)), buffer.data(), IPv6ToString((uint8_t*)&(ip6_header.ip6_dst)));
}
