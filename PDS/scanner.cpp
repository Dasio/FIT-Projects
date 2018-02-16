#include <unistd.h>
#include <csignal>
#include <thread>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "Socket.h"
#include "Scanner.h"

using namespace std;
void printDevices(ostream& stream, const ScanTable &table);

volatile sig_atomic_t signaled = 0;
void signal_handler(int signal)
{
  	signaled = 1;
}

int main(int argc, char **argv)
{
	int c;
	string interface, file_name;
	while ((c = getopt (argc, argv, "i:f:")) != -1)
	{
		switch (c)
		{
			case 'i':
				interface.assign(optarg);
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
	std::signal(SIGINT, signal_handler);
	Scanner scanner(interface);
	std::thread arpReceiver(&Scanner::recvResponses, &scanner);
	std::thread arpSender(&Scanner::sendRequests, &scanner);
	std::thread ipv6(&Scanner::scanIPv6, &scanner);

	arpReceiver.join();
	arpSender.join();
	ipv6.join();

	if (!file_name.empty())
	{
		ofstream file(file_name);
		if (!file.is_open())
		{
			cerr << "Could not open file" << endl;
			return 1;
		}
		printDevices(file, scanner.getTable());
		file.close();
	}
	else
		printDevices(std::cout, scanner.getTable());
	

	return 0;
}

void printDevices(ostream& stream, const ScanTable &table)
{
	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
		<< "<devices>" << std::endl;
	for(auto &device: table)
	{
		// Yes it's ugly
		stream << "    ";
		stream << "<host mac=\"" << device.first << "\">" << std::endl;;
		for(auto &address: device.second.addresses)
		{
			stream << "        ";
			stream << (address.ipv4 ? "<ipv4>" : "<ipv6>") << address.address
				<< (address.ipv4 ? "</ipv4>" : "</ipv6>") << std::endl;
		}
		stream << "    ";
		stream << "</host>" << std::endl;;
	}
	stream << "</devices>" << std::endl;
}
