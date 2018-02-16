#include "Interceptor.h"
#include <csignal>
#include <fstream>
#include <streambuf>
#include <linux/if_ether.h> 
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>


#include <iostream>
#include <stdio.h>

extern volatile sig_atomic_t signaled;
using namespace std;

Interceptor::Interceptor(std::string &interface, std::string &file_name) : 
	socket(ETH_P_ALL)
{
	parseXML(file_name);
	constructPoisonTable();

	socket.loadInterface(interface);
	my_mac = MacBinToInt(socket.src_mac);
}

void Interceptor::run()
{
	PoisonTable::const_iterator it;
	Packet packet;
	while(signaled == 0)
	{
		socket.recvPacket(&packet);
		// Not supported protocol
		if (packet.getIPProtocol() == 0)
			continue;
		// Packet is for us
		if (packet.getDstMac() == my_mac)
		{
			// IPv4
			if (packet.getIPProtocol() == 1)
				it = table.find(packet.getSrcIP());
			// IPv6
			else if (packet.getIPProtocol() == 2)
				it = table.find(IPv6ToString(packet.getSrcIPv6()));
			// IP not found in table
			if (it == table.end())
				continue;
			// Modify destination and send
			packet.setDstMac(it->second);
			packet.setSrcMac(socket.src_mac);
			socket.send(packet);
		}
	}
}

void Interceptor::parseXML(std::string &file_name)
{
	std::string mac_string;
	ipAddress struct_ip;
	xmlInitParser();
	xmlDocPtr doc;
	doc = xmlParseFile(file_name.data());
	if (doc == nullptr)
		return;
	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == nullptr)
	{
		xmlFreeDoc(doc);
		return;
	}
	xmlXPathObjectPtr devices = xmlXPathEvalExpression((xmlChar*)"/devices/host", xpathCtx);
	if (devices == nullptr)
	{
		xmlXPathFreeContext(xpathCtx); 
		xmlFreeDoc(doc);
		return;
	}
	for (int i=0; i < devices->nodesetval->nodeNr; i++) {
		xmlNodePtr host = devices->nodesetval->nodeTab[i];
		xmlChar *mac = xmlGetProp(host, (xmlChar*)"mac");
		if (mac == nullptr)
			continue;
		xmlChar *group = xmlGetProp(host, (xmlChar*)"group");
		mac_string.assign((char*)mac);
		if (group != nullptr)
			parseTable[mac_string].group = std::string((char*)group);
		xmlChar *ip;
		for (xmlNodePtr address = xmlFirstElementChild(host); address != nullptr; address = xmlNextElementSibling(address))
		{
			ip = xmlNodeGetContent(address);
			struct_ip.ipv4 = xmlStrcmp(address->name, (xmlChar*)"ipv4") == 0;
			struct_ip.address = std::string((char*)ip);
			parseTable[mac_string].addresses.insert(struct_ip);
			xmlFree(ip);
		}
		xmlFree(group);
	    xmlFree(mac);
	}
    xmlXPathFreeObject(devices);
	xmlXPathFreeContext(xpathCtx); 
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

bool operator <(const ipAddress& x, const ipAddress& y) {
	return std::tie(x.ipv4, x.address) < std::tie(y.ipv4, y.address);
}

void Interceptor::constructPoisonTable()
{
	for(auto &mac : parseTable)
	{
		// No associated group
		if (mac.second.group.empty())
			continue;
		for(auto &cmp_mac: parseTable)
		{
			// We found pair
			if (mac.first != cmp_mac.first && mac.second.group == cmp_mac.second.group)
			{
				// Iterate each IP
				for(auto &ip: cmp_mac.second.addresses)
				{
					table[ip.address] = MACToInt(mac.first);
				}
			}
		}
	}
}
