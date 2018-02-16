#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "common.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

using namespace std;

class XMLParser
{
public:
	const ScanTable &getTable() { return table; }
	bool parseFile(std::string &file_name);
	void dump(ostream& stream, const ScanTable &table);
	ScanTable table;
private:
	
};

#endif
