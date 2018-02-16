#ifndef TSTREAM_H
#define TSTREAM_H

#include "common.h"
#include "TSPacket.h"
#include "Demultiplexor.h"

class TStream
{
public:
	~TStream();
	void openFile(string);
	void read();
private:
	uint8_t buffer[TSPacket::size];
	Demultiplexor dmx;
	ifstream file;
	ofstream outFile;
};

#endif
