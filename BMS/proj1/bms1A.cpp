#include <iostream>
#include "Encoder.hpp"

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cerr << "File missing" << std::endl;
		return 1;
	}

	Encoder enc;
	if (enc.openFile(argv[1]) == 1)
	{
		std::cerr << "Failed to open file" << std::endl;
		return 1;
	}

	enc.encodeInput();
	enc.saveToFile(std::string(argv[1]).append(".out"));

	return 0;
}
