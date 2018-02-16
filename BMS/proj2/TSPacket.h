#ifndef TSPACKET_H
#define TSPACKET_H

#include "common.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>

class TSPacket
{
public:
	static const unsigned int size = 188;
	TSPacket();
	TSPacket(uint8_t *data, int n);
	
	bool transportError() { return header.transportError; }
	void print() { header.print(); }
	bool isNull() { return header.pid == 0x1FFF; }
	uint16_t getPid() { return header.pid; }
	bool getPayloadStart() { return header.payloadStart == 1; }
	uint8_t getContinuity() { return header.continuityCounter; }
	bool read(ifstream &in);
	bool parse();

	vector<uint8_t> payload;
private:
	struct Header
	{
		uint8_t syncByte : 8;
		uint8_t transportError;
		uint8_t payloadStart: 1;
		uint8_t transportPriority: 1;
		uint16_t pid: 13;
		uint8_t transportScrambling : 2;
		uint8_t adaptionField : 2;
		uint8_t continuityCounter : 4;

		void print()
		{
			cout << "Header:\n"
			<< "\t syncByte = 0x" << std::hex << +syncByte
			<< "\n\t transportError = 0x" << std::hex << +transportError
			<< "\n\t payloadStart = 0x" << std::hex << +payloadStart
			<< "\n\t transportPriority = 0x" << std::hex << +transportPriority
			<< "\n\t pid = 0x" << std::hex << pid
			<< "\n\t transportScrambling = 0x" << std::hex << +transportScrambling
			<< "\n\t adaptionField = 0x" << std::hex << +adaptionField
			<< "\n\t continuityCounter = 0x" << std::hex << +continuityCounter << endl;
		}
	};

	vector<uint8_t> raw;
	Header header;
	vector<uint8_t> adaptionField;
	
	string networkName;
	
};

#endif
