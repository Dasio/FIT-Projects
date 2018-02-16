#ifndef DECODER_HPP
#define DECODER_HPP

#include <fstream>
#include <vector>

class Decoder {
public:
	Decoder();
	~Decoder();

	int openFile(std::string fileName);
	void decodeInput();
	void closeFiles();
private:
	void deInterleave();
	std::ifstream inputFile;
	std::ofstream outputFile;
	std::size_t size = 0;
	std::vector<unsigned char> deInterleaved;
	unsigned char *input = nullptr;
	unsigned char output[256] = {0};
};

#endif
