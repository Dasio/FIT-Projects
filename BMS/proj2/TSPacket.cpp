#include "TSPacket.h"

#include <iomanip>

TSPacket::TSPacket()
{
	raw.resize(TSPacket::size);
}

TSPacket::TSPacket(uint8_t *data, int n)
{
	if(n != size)
		cerr << "Bad size" << endl;
	raw.assign(data, data + n);
}

bool TSPacket::read(ifstream &file)
{
	if(!file.is_open())
		return false;
	file.read(reinterpret_cast<char*>(raw.data()), size);
	return true;
}

bool TSPacket::parse()
{
	adaptionField.clear();
	payload.clear();

	header.syncByte = raw[0];
	if(header.syncByte != 0x47)
	{
		cerr << "Error sync byte\n";
		return false;
	}
	header.transportError = raw[1] >> 7;
	header.payloadStart = raw[1] >> 6 & 0x1;
	header.transportPriority = raw[1] >> 5 & 0x1;
	header.pid = (raw[1] & 0x1F) << 8 | raw[2];
	header.transportScrambling = raw[3] >> 6;
	header.adaptionField = raw[3] >> 4 & 0x3;
	header.continuityCounter = raw[3] & 0xF;

	uint8_t len = 0;
	if(header.adaptionField == 2 || header.adaptionField == 3)
	{
		len = raw[4];
		adaptionField.insert(adaptionField.begin(), raw.begin() + 4, raw.begin() + 4 +  len);
	}
	if (header.payloadStart)
		len++;
	if(header.adaptionField == 1 || header.adaptionField == 3)
	{
		payload.insert(payload.begin(), raw.begin() + 4 + len, raw.end());
	}
	
	return true;

}
