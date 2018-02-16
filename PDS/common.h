#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <string>
#include <set>
#include <map>


typedef uint8_t IPv4[4];
typedef uint8_t IPv6[16];

struct ipAddress
{
	bool ipv4;
	std::string address;
};

struct macProperties
{
	std::string group;
	std::set<ipAddress> addresses;
};

typedef std::map<std::string, macProperties> ScanTable;

std::string getIP(IPv4 ip);
std::string getMAC(uint8_t *mac);

uint8_t IPToBin(const std::string &ip, uint8_t *dst);
//void IPtoBin(const std::string &ip, uint8_t *dst);
void MACtoBin(const std::string &mac, uint8_t *dst);

uint32_t IPv4ToInt(const std::string &ip);
uint64_t MACToInt(const std::string &mac);

uint64_t MacBinToInt(uint8_t *mac);
std::string normalizeIPv6(const std::string &ip);
std::string IPv6ToString(uint8_t *ip);


#endif
