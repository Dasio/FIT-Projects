#include "XMLParser.h"
#include <iostream>

bool XMLParser::parseFile(std::string &file_name)
{
	std::string mac_string;
	ipAddress struct_ip;
	xmlInitParser();
	xmlDocPtr doc;
	doc = xmlParseFile(file_name.data());
	if (doc == nullptr)
		return false;
	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == nullptr)
	{
		xmlFreeDoc(doc);
		return false;
	}
	xmlXPathObjectPtr devices = xmlXPathEvalExpression((xmlChar*)"/devices/host", xpathCtx);
	if (devices == nullptr)
	{
		xmlXPathFreeContext(xpathCtx); 
		xmlFreeDoc(doc);
		return false;
	}
	for (int i=0; i < devices->nodesetval->nodeNr; i++) {
		xmlNodePtr host = devices->nodesetval->nodeTab[i];
		xmlChar *mac = xmlGetProp(host, (xmlChar*)"mac");
		if (mac == nullptr)
			continue;
		xmlChar *group = xmlGetProp(host, (xmlChar*)"group");
		mac_string.assign((char*)mac);
		if (group != nullptr)
			table[mac_string].group = std::string((char*)group);
		xmlChar *ip;
		for (xmlNodePtr address = xmlFirstElementChild(host); address != nullptr; address = xmlNextElementSibling(address))
		{
			ip = xmlNodeGetContent(address);
			struct_ip.ipv4 = xmlStrcmp(address->name, (xmlChar*)"ipv4") == 0;
			struct_ip.address = std::string((char*)ip);
			table[mac_string].addresses.insert(struct_ip);
			xmlFree(ip);
		}
		xmlFree(group);
	    xmlFree(mac);
	}
    xmlXPathFreeObject(devices);
	xmlXPathFreeContext(xpathCtx); 
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return true;
}

void XMLParser::dump(ostream& stream, const ScanTable &table)
{
	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
		<< "<devices>" << std::endl;
	for(auto &device: table)
	{
		// Yes it's ugly
		stream << "    ";
		stream << "<host mac=\"" << device.first << "\"";
		if (!device.second.group.empty())
			stream << " group=\"" << device.second.group << "\"";
		stream << ">" << std::endl;
		for(auto &address: device.second.addresses)
		{
			stream << "        ";
			stream << (address.ipv4 ? "<ipv4>" : "<ipv6>") << address.address
				<< (address.ipv4 ? "</ipv4>" : "</ipv6>") << std::endl;
		}
		stream << "    ";
		stream << "</host>" << std::endl;;
	}
	stream << "</devices>" << std::endl;
}

bool operator <(const ipAddress& x, const ipAddress& y) {
	return std::tie(x.ipv4, x.address) < std::tie(y.ipv4, y.address);
}
