#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <fstream>
#include <vector>

class Encoder {
public:
	Encoder();
	~Encoder();

	int openFile(std::string fileName);
	void saveToFile(std::string fileName);
	void encodeInput();
	void closeFiles();
private:
	std::ifstream inputFile;
	std::size_t size = 0;
	unsigned char *input = nullptr;
	unsigned char output[256];
	std::vector<unsigned char> encoded;;
};

#endif
