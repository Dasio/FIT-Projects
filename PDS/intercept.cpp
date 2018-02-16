#include <iostream>
#include <csignal>
#include <unistd.h>

#include "Interceptor.h"

using namespace std;

volatile sig_atomic_t signaled = 0;
void signal_handler(int signal)
{
  	signaled = 1;
}

int main(int argc, char *argv[])
{
	string interface, file_name;
	int c;
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
	if (interface.empty() || file_name.empty())
	{
		cerr << "You need specifiy interface and input file" << endl;
		return 1;
	}
	std::signal(SIGINT, signal_handler);
	Interceptor interceptor(interface, file_name);
	interceptor.run();
	return 0;
}
