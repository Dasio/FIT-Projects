#include <iostream>
#include "Decoder.hpp"

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cerr << "File missing" << std::endl;
		return 1;
	}
	Decoder dec;
	if (dec.openFile(argv[1]) == 1)
	{
		std::cerr << "Failed to open file" << std::endl;
		return 1;
	}

	dec.decodeInput();
	
	
	return 0;
}
