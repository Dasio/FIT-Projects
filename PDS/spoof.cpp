#include <iostream>
#include <string>
#include <cstring>
#include "common.h"
#include "Spoofer.h"
#include <getopt.h>
#include <stdio.h>
#include <csignal>

using namespace std;

enum {
	Interface,
	Time,
	Protocol,
	Victim1IP,
	Victim1MAC,
	Victim2IP,
	Victim2MAC
};
volatile sig_atomic_t signaled = 0;
void signal_handler(int signal)
{
  	signaled = 1;
}

int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"i", required_argument, 0, Interface},
		{"t", required_argument, 0, Time},
		{"p", required_argument, 0, Protocol},
		{"victim1ip", required_argument, 0, Victim1IP},
		{"victim1mac", required_argument, 0, Victim1MAC},
		{"victim2ip", required_argument, 0, Victim2IP},
		{"victim2mac", required_argument, 0, Victim2MAC},
		{0, 0, 0, 0}
	};
	string interface, protocol;
	string victim1ip, victim1mac, victim2ip, victim2mac;
	uint32_t sec = 5000;
	int c;
	while((c = getopt_long_only(argc, argv, "", long_options, nullptr)) != -1)
	{
		switch (c)
		{
			case Interface:
				interface.assign(optarg);
				break;
			case Time:
				sec = strtol(optarg, nullptr, 10);
				break;
			case Protocol:
				protocol.assign(optarg);
				break;
			case Victim1IP:
				victim1ip.assign(optarg);
				break;
			case Victim1MAC:
				victim1mac.assign(optarg);
				break;
			case Victim2IP:
				victim2ip.assign(optarg);
				break;
			case Victim2MAC:
				victim2mac.assign(optarg);
				break;
		}
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

	std::signal(SIGINT, signal_handler);
	Spoofer spoofer(interface, sec, p, victim1ip, victim1mac, victim2ip, victim2mac);
	spoofer.run();

	return 0;
}
