#include "common.h"
#include <iostream>
#include "Demultiplexor.h"
#include <string>
#include <fstream>
#include "TStream.h"

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		std::cerr << "File missing" << std::endl;
		return 1;
	}
	TStream stream;
	stream.openFile(argv[1]);
	stream.read();
}
