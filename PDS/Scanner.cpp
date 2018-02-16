#include "Scanner.h"
#include <list>
#include <numeric>
#include <iostream>
#include <arpa/inet.h>
#include <csignal>
#include <linux/if_ether.h>  
#include <chrono>
#include <thread>
#include "ICMPv6.h"

extern volatile sig_atomic_t signaled;

Scanner::Scanner(std::string &interface) : arpSocket(ETH_P_ARP), ip6Socket(ETH_P_IPV6)
{
	arpSocket.loadInterface(interface);
	ip6Socket.loadInterface(interface);
}

void Scanner::sendRequests()
{
	std::pair<uint32_t, uint32_t> range = arpSocket.getRange();
	uint32_t from = range.first;
	uint32_t to = range.second;
	Arp arp;
	arp.request(arpSocket.src_ip, arpSocket.src_mac, arpSocket.netmask);
	uint32_t count = to - from + 1;

	std::list<uint32_t> l(count);
    std::iota(l.begin(), l.end(), from);

    IPv4 ip;
    for(auto n: l)
    {
    	ip[0] = n >> 24; ip[1] = n >> 16; ip[2] = n >> 8; ip[3] = n;
    	arp.setDstIP(ip);
    	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    	arpSocket.send(arp);
    	if (signaled == 1)
    		break;
    }
}

void Scanner::recvResponses()
{
	ipAddress ip = { true };
	Arp::arpHeader *header;

	do
	{
		header = arpSocket.receive();
		if (header == nullptr)
			break;
		if (header->op_code != htons(2))
			continue;
		ip.address = getIP(header->src_protocol);
		table[getMAC(header->src_hw)].addresses.insert(ip);
		if (signaled == 1)
			break;

	} while (header != nullptr);

}

void Scanner::scanIPv6()
{
	ipAddress ip = { false };
	Packet recv_packet;
	ICMPv6 packet;
	packet.echoRequest(ip6Socket.src_mac, ip6Socket.ipv6);
	ip6Socket.send(packet);
	uint16_t size;
	uint8_t counter = 0;
	do
	{
		size = ip6Socket.recvPacket(&recv_packet);
		if (size == 0)
		{
			if (++counter == 5)
				break;
			else
				continue;
		}
		ip.address = IPv6ToString(recv_packet.getSrcIPv6());
		table[getMAC(recv_packet.getSrcMac())].addresses.insert(ip);

	} while (signaled == 0);
}

bool operator <(const ipAddress& x, const ipAddress& y) {
	return std::tie(x.ipv4, x.address) < std::tie(y.ipv4, y.address);
}
