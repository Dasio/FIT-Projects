#include "Encoder.hpp"
#include "common.hpp"
#include <iostream>
#include <cmath>

Encoder::Encoder()
{
	initialize_ecc();
}

Encoder::~Encoder()
{
	closeFiles();
	if (input)
		delete[] input;
}

void Encoder::closeFiles()
{
	if (inputFile.is_open())
		inputFile.close();
}

int Encoder::openFile(std::string fileName)
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
	return 0;
}

void Encoder::encodeInput()
{
	unsigned mapIndex;
	size_t encodedLength = 0;
	size_t dataLength = dataSize;

	size_t encodedSize = (size / dataSize) * blockSize;
	size_t lastBlockSize = size % dataSize + paritySize;
	if (size % dataSize != 0)
		encodedSize += lastBlockSize;

	size_t blocks = std::ceil(encodedSize / (double)blockSize);
	encoded.clear();
	encoded.resize(encodedSize);
	unsigned blocksCounter = 0;

	while(encodedLength < size)
	{
		// For last block
		if(dataLength + encodedLength > size)
			dataLength = size - encodedLength;
		encode_data(input + encodedLength, dataLength, output);
		// Save encoded data to vector + interleave it
		for(int i=0; i<dataLength + paritySize; ++i)
		{
			mapIndex = blocksCounter + i * blocks;
			// Need to adjust index if last codeword is smaller
			if(i > lastBlockSize)
				mapIndex -= i - lastBlockSize;;
			encoded[mapIndex] = output[i];
		}
		encodedLength += dataSize;
		// Move to next codeword
		++blocksCounter;
		// Without interleaving
		//encoded.insert(encoded.end(), output, output + dataLength + paritySize);

	}
}

void Encoder::saveToFile(std::string fileName)
{
	std::ofstream outf(fileName, std::ofstream::binary);
	outf.write((const char *)&encoded.front(), encoded.size());
	outf.close();
}
