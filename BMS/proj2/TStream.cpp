#include "TStream.h"

TStream::~TStream()
{
	if(file.is_open())
		file.close();
	if(outFile.is_open())
		outFile.close();
}

void TStream::openFile(string fileName)
{
	file.open(fileName, ios::binary);

	// Create name for output file
	size_t index = fileName.find_last_of(".");
	std::string outFileName = fileName.substr(0, index).append(".txt");
	// Open file for writing
	outFile.open(outFileName);
}

void TStream::read()
{
	while(!file.eof())
	{
		file.read(reinterpret_cast<char*>(buffer), TSPacket::size);
		if(file.gcount() != TSPacket::size)
			break;
		dmx.packets++;
		TSPacket packet(buffer, file.gcount());
		packet.parse();
		dmx.packetsCounter[packet.getPid()]++;
		// Skip broken packet
		if (packet.transportError() || packet.isNull())
			continue;
		if (!dmx.analyzedPAT && packet.getPid() == TS_PID_PAT)
		{
			dmx.parsePAT(packet);
		}
		else if(!dmx.analyzedNIT && packet.getPid() == dmx.NITPid)
		{
			dmx.parseNIT(packet);
		}
		else if(packet.getPid() == TS_PID_SDT && !dmx.analyzedSDT)
		{
			dmx.parseSDT(packet);
		}
		else if(dmx.analyzedPAT && dmx.isPMTPacket(packet.getPid()))
		{
			dmx.parsePMT(packet);
		}
		
	}
	dmx.calculateBitrate();
	dmx.bindMaps();
	dmx.printInfo(outFile);
}
