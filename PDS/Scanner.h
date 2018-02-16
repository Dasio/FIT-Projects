#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include <string>
#include "Socket.h"
#include <map>
#include <set>

class Scanner
{
public:
	Scanner(std::string &interface);
	const ScanTable &getTable() { return table; }
	/**
	 * @brief Starts sending ARP requestes on each IP in subnet
	 */
	void sendRequests();

	/**
	 * @brief Receive ARP response and save it to table
	 */
	void recvResponses();

	/**
	 * @brief Send echo request and afterwards receiveing and saving response to table
	 */
	void scanIPv6();
private:
	Socket arpSocket;
	Socket ip6Socket;
	ScanTable table;
};

#endif

