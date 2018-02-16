#ifndef SPOOFER_H
#define SPOOFER_H

#include "common.h"
#include "Socket.h"
#include <utility>
#include "ICMPv6.h"

using namespace std;
struct Device
{
	std::string mac;
	std::set<ipAddress> addresses;
};
typedef std::vector<std::pair<Device, Device>> Devices;

class Spoofer
{
public:
	
	enum Protocol
	{
		ARP,
		NDP
	};
	Spoofer(string &interface, uint32_t sec, Protocol p, string &victim1ip, string &victim1mac, string &victim2ip, string &victim2mac);
	Spoofer(string &interface, uint32_t sec, Protocol p);
	/**
	 * @brief Run Spoofer
	 */
	void run();
	void massRun(Devices &devices);
	
	void massSend(std::vector<Packet> &packets);
	/**
	 * @brief Flooding ARP
	 * 
	 * @param ip
	 * @param dst_ip
	 * @param dst_mac
	 */
	void poisonARP(uint8_t *ip, uint8_t *dst_ip, uint8_t *dst_mac);

	/**
	 * @brief Flooding NDP
	 * 
	 * @param ip
	 * @param dst_ip
	 * @param dst_mac
	 */
	void poisonNDP(std::string ip, std::string dst_ip, uint8_t *dst_mac);

	/**
	 * @brief Send original ARP packets
	 */
	void refreshARPCache();

	/**
	 * @brief Send original NDP packets
	 */
	void refreshNDPCache();
private:
	void massRunARP(std::pair<Device, Device> &group);
	void massRunNDP(std::pair<Device, Device> &group);
	std::vector<Arp> prepareARPPackets(std::pair<Device, Device> &group);
	std::vector<ICMPv6> prepareNDPPackets(std::pair<Device, Device> &group);
	Socket socket;
	string interface;
	uint32_t sec;
	Protocol protocol;
	uint8_t vic1_ip[16];
	uint8_t vic1_mac[6];
	uint8_t vic2_ip[16];
	uint8_t vic2_mac[6];
	std::string ip_1;
	std::string ip_2;
};

#endif
