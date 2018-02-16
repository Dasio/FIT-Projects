#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <iostream>
#include <vector>

// Bad practice, but whatever, for school project it's fine
using namespace std;

const uint8_t TS_PID_PAT = 0x00;
const uint8_t TS_PID_CAT = 0x01;
const uint8_t TS_PID_NIT = 0x10;
const uint8_t TS_PID_SDT = 0x11;

const uint8_t TS_TABLE_PAT = 0x00;
const uint8_t TS_TABLE_CAT = 0x01;
const uint8_t TS_TABLE_PMT = 0x02;
const uint8_t TS_TABLE_NIT = 0x40;
const uint8_t TS_TABLE_SDT = 0x42;

const uint8_t TS_DESCRIPTOR_NETWORK_NAME = 0x40;
const uint8_t TS_DESCRIPTOR_SERVICE = 0x48;
const uint8_t TS_DESCRIPTOR_TERRESTRIAL = 0x5A;

const uint8_t TS_STREAM_VIDEO = 0x02;
const uint8_t TS_STREAM_AUDIO = 0x03;

const uint8_t TS_SERVICE_DTV = 0x01;


#endif
