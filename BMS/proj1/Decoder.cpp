#include "Decoder.hpp"
#include "common.hpp"
#include <iostream>
#include <cmath>

Decoder::Decoder()
{
	initialize_ecc();
}


Decoder::~Decoder()
{
	closeFiles();
	if (input)
		delete[] input;
}

void Decoder::closeFiles()
{
	if (inputFile.is_open())
		inputFile.close();
	if (outputFile.is_open())
		outputFile.close();
}

int Decoder::openFile(std::string fileName)
{
	inputFile.open(fileName, std::ifstream::binary);
	if (!inputFile.is_open())
		return 1;
	// Get length of file
	inputFile.seekg(0, inputFile.end);
	std::size_t newSize = inputFile.tellg();
	inputFile.seekg(0, inputFile.beg);

	if(newSize > size)
	{
		delete[] input;
		size = newSize;
		input = new unsigned char[size];
	}

	inputFile.read(reinterpret_cast<char*>(input), size);
	deInterleaved.resize(size);
	// Open file for output
	outputFile.open(fileName.append(".ok"), std::ofstream::binary);
	return 0;
}

void Decoder::decodeInput()
{
	deInterleave();
	size_t decoded = 0;
	size_t blockLength = blockSize;
	size_t writeSize = dataSize;

	while(decoded < size)
	{
		// If it is last block, adjust size
		if (blockLength + decoded >= size)
		{
			blockLength = size - decoded;
			writeSize = blockLength - paritySize;
		}
		// Decode date
		decode_data(deInterleaved.data() + decoded, blockLength);
		// Corrector errors
		if (check_syndrome() != 0)
			correct_errors_erasures(deInterleaved.data() + decoded, blockLength, 0, nullptr);

		outputFile.write(reinterpret_cast<char*>(deInterleaved.data() + decoded), writeSize);
		decoded += blockLength;
	}
	
}

void Decoder::deInterleave()
{
	size_t blocks = std::ceil(size/(double)blockSize);
	size_t lastBlockSize = size % blockSize;
	size_t dataLength = dataSize;
	for(int j=0; j< blocks; j++) {
		if(j == blocks - 1) {
			dataLength = lastBlockSize - paritySize;
		}
		for(int i=0; i<dataLength + paritySize; ++i)
		{
			int map = j + i * blocks;
			if(i > lastBlockSize)
				map -= i - lastBlockSize;
			deInterleaved[i + j * blockSize] = input[map];
		}
	}
}
