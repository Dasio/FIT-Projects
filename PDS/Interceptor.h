#ifndef INTERCEPTOR_H
#define INTERCEPTOR_H

#include <string>
#include "common.h"
#include "Socket.h"
#include <unordered_map>

typedef std::unordered_map<std::string, uint64_t> PoisonTable;
class Interceptor
{
public:
	Interceptor(std::string &interface, std::string &file_name);

	/**
	 * @brief Run network interceptor on specified interface
	 */
	void run();
private:
	Socket socket;
	uint64_t my_mac;
	void parseXML(std::string &file_name);
	void constructPoisonTable();
	ScanTable parseTable;
	PoisonTable table;
};

#endif
