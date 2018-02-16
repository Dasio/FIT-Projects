#include "common.h"
#include <iomanip>
#include <cstring>
#include <sstream>
#include <arpa/inet.h>
#include <tuple>
#include <iostream>

std::string getIP(IPv4 ip)
{
	std::string strIP;
	for(uint8_t i=0; i<4; ++i)
	{
		if (i != 0)
			strIP.push_back('.');
		strIP.append(std::to_string(ip[i]));
	}
	

	return strIP;
}

std::string getMAC(uint8_t *mac)
{
	std::stringstream stream;
	for(uint8_t i=0; i<6; ++i)
	{
		if (i != 0 && i%2 ==0)
			stream << ".";
		stream << std::setfill('0') << std::setw(2) << std::hex << (int)mac[i];
	}
	return stream.str();
}

uint8_t IPToBin(const std::string &ip, uint8_t *dst)
{
	struct in6_addr ipv6;
	struct in_addr ipv4;
	int result;
	//std::cout << ip << std::endl;
	result = inet_pton(PF_INET, ip.data(), &ipv4);
	// Success
	if (result == 1)
	{	memcpy(dst, &ipv4, 4);
		return 1;
	}
	result = inet_pton(PF_INET6, ip.data(), &ipv6);
	// Success
	if (result == 1)
	{	memcpy(dst, &ipv6, 16);
		return 2;
	}
	return 0;
}
/*void IPtoBin(const std::string &ip, uint8_t *dst)
{
	uint32_t add = inet_addr(ip.data());
	dst[3] = add >> 24; dst[2] = add >> 16; dst[1] = add >> 8; dst[0] = add;

}*/

uint32_t IPv4ToInt(const std::string &ip)
{
	return inet_addr(ip.data());
}

void IPv6ToInt(const std::string &ip, uint64_t *first_half, uint64_t *second_half)
{
	struct in6_addr result;
	inet_pton(AF_INET6, ip.data(), &result);
	uint8_t *d = result.s6_addr;
	*first_half = ((uint64_t)d[0]) << 56 | ((uint64_t)d[1]) << 48 | ((uint64_t)d[2]) << 40 |
		((uint64_t)d[3]) << 32 | ((uint64_t)d[4]) << 24 | ((uint64_t)d[5]) << 16 |
		((uint64_t)d[6]) << 8 | ((uint64_t)d[7]);
	*second_half = ((uint64_t)d[8]) << 56 | ((uint64_t)d[9]) << 48 | ((uint64_t)d[10]) << 40 |
		((uint64_t)d[11]) << 32 | ((uint64_t)d[12]) << 24 | ((uint64_t)d[13]) << 16 |
		((uint64_t)d[14]) << 8 | ((uint64_t)d[15]);
}
void MACtoBin(const std::string &mac, uint8_t *dst)
{
	dst[0] = strtol(mac.substr(0,2).data(), nullptr, 16);
	dst[1] = strtol(mac.substr(2,2).data(), nullptr, 16);
	dst[2] = strtol(mac.substr(5,2).data(), nullptr, 16);
	dst[3] = strtol(mac.substr(7,2).data(), nullptr, 16);
	dst[4] = strtol(mac.substr(10,2).data(), nullptr, 16);
	dst[5] = strtol(mac.substr(12,2).data(), nullptr, 16);
}

uint64_t MACToInt(const std::string &mac)
{
	uint8_t mac_tmp[6];
	MACtoBin(mac, mac_tmp);
	return MacBinToInt(mac_tmp);
}

inline uint64_t MacBinToInt(uint8_t *mac)
{
	return ((uint64_t)mac[0]) << 40 | ((uint64_t)mac[1]) << 32 | ((uint64_t)mac[2]) << 24 | 
		((uint64_t)mac[3]) << 16 | ((uint64_t)mac[4]) << 8 | ((uint64_t)mac[5]);
}

std::string IPv6ToString(uint8_t *ip)
{
	struct in6_addr *result = reinterpret_cast<struct in6_addr*>(ip);
	char char_ip[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, result, char_ip, INET6_ADDRSTRLEN);
	return char_ip;
}


std::string normalizeIPv6(const std::string &ip)
{
	struct in6_addr result;
	char char_ip[INET6_ADDRSTRLEN];
	inet_pton(AF_INET6, ip.data(), &result);
	inet_ntop(AF_INET6, &result, char_ip, INET6_ADDRSTRLEN);
	return char_ip;
}
