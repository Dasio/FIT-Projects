#ifndef DEMULTIPLEXOR_H
#define DEMULTIPLEXOR_H

#include <string>
#include <fstream>
#include <map>
#include "TSPacket.h"
#include "common.h"
#include <set>

class Demultiplexor
{
public:
	Demultiplexor();
	void calculateBitrate();
	bool parseNIT(TSPacket &packet);
	bool parsePAT(TSPacket &packet);
	bool parseSDT(TSPacket &packet);
	bool parsePMT(TSPacket &packet);

	// Check in PAT table if there is packet
	bool isPMTPacket(uint16_t pid);
	// Print info about transport stream
	void printInfo(ostream& stream);
	// Used for bind tables
	void bindMaps();
	uint16_t NITPid;
	bool analyzedNIT;
	bool analyzedPAT;
	bool analyzedSDT;
	// pid, packets
	map<uint16_t, uint32_t> packetsCounter;
	// From PMT packets
	map<uint16_t, set<uint16_t>> programElementary;
	// Total packets
	uint32_t packets;

	string getBandwidth() { return bandwidth < bandwidthTable.size() ? bandwidthTable[bandwidth] : "Unknown"; }
	string getConstellation() { return constellation < constellationTable.size() ? constellationTable[constellation] : "Unknown"; }
	string getGuardInterval() { return guardInterval < guardIntervalTable.size() ? guardIntervalTable[guardInterval] : "Unknown"; }
	string getCodeRate() { return codeRateHP < codeRateTable.size() ? codeRateTable[codeRateHP] : "Unknown"; }
	long double getBandwidthValue() { return bandwidthValues[bandwidth]; }
	long double getModulation() {
		switch(constellation)
		{
			case 0:
				return 1/(long double)4;
			case 1:
				return 1/(long double)2.0;
			case 2:
				return 3/(long double)4.0;
		}
		return 0.0;
	}
	long double getCodeRateValue() {
		switch(codeRateHP)
		{
			case 0:
				return 1/(long double)2.0;
			case 1:
				return 2/(long double)3.0;
			case 2:
				return 3/(long double)4.0;
			case 3:
				return 5/(long double)6.0;
			case 4:
				return 7/(long double)8.0;
		}
		return 0.0;
	}
	// For calculation
	long double getGuardValue() {
		switch(guardInterval)
		{
			case 0:
				return 32/(long double)33.0;
			case 1:
				return 16/(long double)17.0;
			case 2:
				return 8/(long double)9.0;
			case 3:
				return 4/(long double)5.0;
		}
		return 0.0;
	}
private:
	bool parseDescriptors(TSPacket &packet, uint8_t offset, uint16_t length);
	void parseServiceDescriptor(uint16_t offset, uint16_t length, uint16_t serviceID);
	std::ofstream outFile;
	map<uint16_t, uint16_t> PAT;
	bool newSDT;
	bool parse;
	string networkName;
	uint16_t networkID;
	uint8_t bandwidth;
	uint8_t constellation;
	uint8_t codeRateHP;
	//uint8_t codeRateLP;
	uint8_t guardInterval;
	string serviceProviderName;
	string serviceName;
	
	unsigned long totalBitrate;

	vector<uint8_t> sdtData;
	vector<long double> bandwidthValues = 
	{
		8.0, 7.0, 6.0, 5.0, 1.7
	};
	vector<string> bandwidthTable = 
	{
		"8 MHz",
		"7 MHz",
		"6 MHz",
		"5 MHz",
		"1,7 MHz"	
	};

	vector<string> constellationTable = 
	{
		"QPSK",
		"16-QAM",
		"64-QAM"
	};

	vector<string> codeRateTable = 
	{
		"1/2",
		"2/3",
		"3/4",
		"5/6",
		"7/8"	
	};

	vector<string> guardIntervalTable = 
	{
		"1/32",
		"1/16",
		"1/8",
		"1/4"
	};

	struct ProgramInfo
	{
		uint16_t pid;
		uint16_t serviceID;
		string serviceProviderName;
		string serviceName;
		long double bitrate;

		bool operator<(const ProgramInfo &a) const
		{
			return pid < a.pid;
		}

	};
	vector<ProgramInfo> programs;

};

#endif
