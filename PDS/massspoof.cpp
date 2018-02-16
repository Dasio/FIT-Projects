#include <iostream>
#include <csignal>
#include <unistd.h>
#include <string>
#include "Spoofer.h"
#include <utility>
#include "XMLParser.h"
#include <unordered_map>

volatile sig_atomic_t signaled = 0;
void signal_handler(int signal)
{
  	signaled = 1;
}

using namespace std;

int main(int argc, char *argv[])
{
	int c;
	string interface, file_name, protocol;
	uint32_t sec = 5000;
	while ((c = getopt (argc, argv, "i:f:p:t:")) != -1)
	{
		switch (c)
		{
			case 'i':
				interface.assign(optarg);
				break;
			case 'p':
				protocol.assign(optarg);
				break;
			case 't':
				sec = strtol(optarg, nullptr, 10);
				break;
			case 'f':
				file_name.assign(optarg);
				break;
			default:
				cerr << "Unknown argument" << endl;
				return 1;
		}
	}
	if (interface.empty())
	{
		cerr << "You have to specify interface" << endl;
		return 1;
	}

	if(file_name.empty())
	{

		cerr << "You have to specify file name" << endl;
		return 1;
	}
	Spoofer::Protocol p;
	if (protocol.compare("arp") == 0)
		p = Spoofer::Protocol::ARP;
	else if (protocol.compare("ndp") == 0)
		p = Spoofer::Protocol::NDP;
	else
	{
		cerr << "Wrong protocol" << endl;
		return 1;
	}

	XMLParser parser;
	if (!parser.parseFile(file_name))
	{
		cerr << "Could'nt parse file" << endl;
		return 1;
	}
	Devices devices;
	std::unordered_map<std::string, bool> grouped;
	for(auto &mac : parser.table)
	{
		// No associated group
		if (mac.second.group.empty())
			continue;
		// Already grouped up
		if (grouped.find(mac.first) != grouped.end())
			continue;
		for(auto &cmp_mac: parser.table)
		{
			// We found pair
			if (mac.first != cmp_mac.first && mac.second.group == cmp_mac.second.group)
			{
				Device dev1, dev2;
				dev1.mac = mac.first;
				dev2.mac = cmp_mac.first;
				dev1.addresses = mac.second.addresses;
				dev2.addresses = cmp_mac.second.addresses;
				devices.push_back(std::make_pair(dev1, dev2));
				grouped[mac.first] = true;
				grouped[cmp_mac.first] = true;
			}
		}
	}
	std::signal(SIGINT, signal_handler);
	Spoofer spoofer(interface, sec, p);
	spoofer.massRun(devices);
	return 0;
}
