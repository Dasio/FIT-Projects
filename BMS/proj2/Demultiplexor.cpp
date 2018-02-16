#include "Demultiplexor.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

Demultiplexor::Demultiplexor() : NITPid(TS_PID_NIT), analyzedNIT(false), analyzedPAT(false), analyzedSDT(false),
  packets(0), newSDT(false), parse(false)
{

}



bool Demultiplexor::parsePAT(TSPacket &packet)
{
	uint8_t tableID = packet.payload[0];
	if(tableID != TS_TABLE_PAT)
	{
		cerr << "Table ID shall be set 0x00" << endl;
		return false;
	}
	uint16_t sectionLength = (packet.payload[1] & 0xF) << 8  | packet.payload[2];
	//uint16_t tStreamID = packet.payload[3] << 8 | packet.payload[4];
	//uint8_t sectionNumber = packet.payload[6];
	//uint8_t lastSectionNumber = packet.payload[7];
	uint8_t index = 0;
	uint16_t programNumber;
	uint16_t pid;
	while(index < sectionLength)
	{
		programNumber = packet.payload[index + 8] << 8 | packet.payload[index + 9];
		pid = (packet.payload[index + 10] & 0x1F) << 8 | packet.payload[index + 11];
		PAT.insert(make_pair(programNumber, pid));
		// Update NIT Pid
		if(programNumber == 0x00)
			NITPid = pid;
		index += 4;
	}

	analyzedPAT = true;

	return true;
}

bool Demultiplexor::isPMTPacket(uint16_t pid)
{
	if(pid == NITPid)
		return false;

	for(auto &kv: PAT)
		if (kv.second == pid)
			return true;
	return false;
}

bool Demultiplexor::parseNIT(TSPacket &packet)
{
	uint8_t tableID = packet.payload[0];
	if(tableID != TS_TABLE_NIT)
	{
		cerr << "Wrong NIT Table id" << endl;
		return false;
	}
	//uint16_t sectionLength = (packet.payload[1] & 0xF) << 8 | packet.payload[2];
	networkID = packet.payload[3] << 8 | packet.payload[4];
	
	//uint8_t versionNumber = (packet.payload[5] >> 1) & 0x31;
	//uint8_t currentNextIndicator = packet.payload[5] & 0x01;
	//uint8_t sectionNumber = packet.payload[6];
	//uint8_t lastSectionNumber = packet.payload[7];
	uint16_t networkLength = (packet.payload[8] & 0xF) << 8 | packet.payload[9];

	parseDescriptors(packet, 10, networkLength);

	uint16_t transportStreamLength = (packet.payload[networkLength + 10] & 0xF) << 8 | packet.payload[networkLength + 11];
	uint8_t payloadIndex = networkLength + 12;
	uint8_t i = 0;
	while(i < transportStreamLength)
	{
		//uint16_t transportStreamID = packet.payload[payloadIndex] << 8 | packet.payload[payloadIndex + 1];
		payloadIndex += 2;
		//uint16_t originalNetworkID = packet.payload[payloadIndex] << 8 | packet.payload[payloadIndex + 1];
		payloadIndex += 2;
		uint16_t transportDescriptorLen = (packet.payload[payloadIndex] & 0xF) << 8 | packet.payload[payloadIndex + 1];
		payloadIndex += 2;;
		parseDescriptors(packet, payloadIndex, transportDescriptorLen);

		i += transportDescriptorLen + 6;
	}

	analyzedNIT = true;

	return true;
}


bool Demultiplexor::parseSDT(TSPacket &packet)
{
	/*uint8_t tableID = packet.payload[0];
	if(tableID != TS_TABLE_SDT)
	{
		cerr << "Wrong SDT Table id" << endl;
		return false;
	}*/
	uint16_t sectionLength = (packet.payload[1] & 0xF) << 8 | packet.payload[2];
	//uint16_t tStreamID = packet.payload[3] << 8 | packet.payload[4];
	uint16_t index = 0;
	uint16_t serviceID;
	uint16_t desLength;
	if(packet.getPayloadStart())
	{
		if(newSDT)
		{
			parse = true;	
		}
		newSDT = true;
	}
	
	if(!parse)
	{
		sdtData.insert(sdtData.end(), packet.payload.begin(), packet.payload.end());
	}
	if(parse)
	{
		while(index < sectionLength)
		{
			serviceID = sdtData[index + 11] << 8 | sdtData[index + 11 + 1];
			desLength = (sdtData[index + 11 + 3] & 0xF) << 8 | sdtData[index + 11 + 4];
			parseServiceDescriptor(index + 11 + 5, desLength, serviceID);
			index += desLength + 5;

		}
	}
	else
	{
		return false;
	}
	parse = false;
	analyzedSDT = true;
	
	return true;
}

bool Demultiplexor::parsePMT(TSPacket &packet)
{
	uint8_t tableID = packet.payload[0];
	if(tableID != TS_TABLE_PMT)
	{
		cerr << hex << "PID=0x" << packet.getPid() << endl;
		cerr << "PMT wrong table ID" << endl;
		return false;
	}
	uint16_t sectionLength = (packet.payload[1] & 0xF) << 8 | packet.payload[2];
	uint16_t programNumber = packet.payload[3] << 8 | packet.payload[4];
	uint16_t programInfoLength = (packet.payload[10] & 0xF) << 8 | packet.payload[11];
	
	uint8_t payloadIndex = programInfoLength + 12;
	uint8_t i = 0;
	//uint8_t streamType;
	uint16_t elementaryPID;
	uint16_t ESInfoLength;

	while (i < sectionLength)
	{
		//streamType = packet.payload[payloadIndex + i];
		elementaryPID = (packet.payload[payloadIndex + i + 1] & 0x1F) << 8 | packet.payload[payloadIndex + i + 2];
		ESInfoLength = (packet.payload[payloadIndex + i + 3] & 0xF) << 8 | packet.payload[payloadIndex + i + 4];
	
		if(elementaryPID != 0x1FFF)
			programElementary[programNumber].insert(elementaryPID);
		i += ESInfoLength + 5;
	}
	return true;

}
void Demultiplexor::parseServiceDescriptor(uint16_t offset, uint16_t length, uint16_t serviceID)
{
	uint16_t index = 0;
	uint8_t len;
	//uint8_t serviceType;
	uint8_t serviceProviderNameLength;
	uint8_t serviceNameLength;
	ProgramInfo info;
	while(index < length)
	{
		len = sdtData[index + offset + 1];
		switch(sdtData[index + offset])
		{
			case TS_DESCRIPTOR_SERVICE:
				//serviceType = sdtData[index + offset + 2];
				serviceProviderNameLength = sdtData[index + offset + 3];
				info.serviceProviderName.assign(reinterpret_cast<char*>(sdtData.data()) + index + offset + 4, serviceProviderNameLength);
				serviceNameLength = sdtData[index + offset + serviceProviderNameLength + 4];
				info.serviceName.assign(reinterpret_cast<char*>(sdtData.data()) + index + offset + serviceProviderNameLength + 5, serviceNameLength);
				info.serviceID = serviceID;
				programs.push_back(info);
				break;
			default:
				break;
		}
		break;
		index += len + 2;
	}

}
bool Demultiplexor::parseDescriptors(TSPacket &packet, uint8_t offset, uint16_t length)
{
	uint16_t index = 0;
	uint8_t len;
	//uint32_t freq;

	while(index < length)
	{
		len = packet.payload[index + offset + 1];
		switch(packet.payload[index + offset])
		{
			case TS_DESCRIPTOR_NETWORK_NAME:
				networkName.assign(reinterpret_cast<char*>(packet.payload.data()) + index + offset + 2, len);
				break;
			case TS_DESCRIPTOR_TERRESTRIAL:
				//freq = packet.payload[index + offset + 2] << 24 | packet.payload[index + offset + 3] << 16 |packet.payload[index + offset + 4] << 8 | packet.payload[index + offset + 5]; 
				bandwidth = packet.payload[index + offset + 6] >> 5;
				constellation = packet.payload[index + offset + 7] >> 6;
				codeRateHP = packet.payload[index + offset + 7] & 0x7;
				//codeRateLP = packet.payload[index + offset + 8] >> 5;
				guardInterval = (packet.payload[index + offset + 8] >> 3) & 0x3;
				break;
			default:
				break;
		}
		index += len + 2;
	}
	return true;
}
void Demultiplexor::bindMaps()
{
	// Delete unknown packets
	for(auto &p: programElementary)
	{
		for(auto &e: p.second)
		{
			if(packetsCounter.find(e) == packetsCounter.end())
			{
				p.second.erase(e);
			}
		}
	}

	// Map programs and calculate bitrate
	for(auto &program: programs)
	{
		program.pid = PAT[program.serviceID];
		for(auto &epid: programElementary[program.serviceID])
		{
			packetsCounter[program.pid] += packetsCounter[epid];
			
		}
		program.bitrate = (packetsCounter[program.pid] / (long double)packets) * totalBitrate;
		program.bitrate /= 1000 * 1000;
	}
	// Sort by pid
	sort(programs.begin(), programs.end());
}
// http://www.dveo.com/broadcast/bit-rate-calculation-for-COFDM-modulation.shtml
void Demultiplexor::calculateBitrate()
{
	long double A = 49764705.88;
	long double bandwidth = getBandwidthValue() / 8;
	long double B = A * bandwidth;
	long double C = B * getModulation();
	long double D = C * getCodeRateValue();
	totalBitrate = D * getGuardValue();
	
}
void Demultiplexor::printInfo(ostream& stream)
{
	stream << "Network name: " << networkName << endl
		<< "Network ID: " << networkID << endl
		<< "Bandwidth: " << getBandwidth() << endl
		<< "Constellation: " << getConstellation() << endl
		<< "Guard interval: " << getGuardInterval() << endl
		<< "Code rate: " << getCodeRate() << endl;

	stream << endl;

	for(auto &program: programs)
		stream << "0x" << hex << setw(4) << setfill('0') << program.pid
		<< "-" << program.serviceProviderName << "-" << program.serviceName
		<< ": " << fixed << setprecision(2) << program.bitrate << " Mbps"
		<< endl;

}
