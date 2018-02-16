#include "XMLParser.h"
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
void printTable(ScanTable &table);
unordered_map<uint16_t, string> indexs;

int main(int argc, char *argv[])
{
	int c;
	string in_file, out_file;
	while ((c = getopt (argc, argv, "f:o:")) != -1)
	{
		switch (c)
		{
			case 'f':
				in_file.assign(optarg);
				break;
			case 'o':
				out_file.assign(optarg);
				break;
			default:
				cerr << "Unknown argument" << endl;
				return 1;
		}
	}
	string cmd;
	XMLParser parser;
	parser.parseFile(in_file);
	ofstream out(out_file);
	if (!out.is_open())
	{
		cerr << "Could'n open output file" << endl;
		return 1;
	}

	uint16_t first, second;
	string group;
	do
	{
		printTable(parser.table);
		cout << endl;
		cout << "Enter index of first MAC address: ";
		getline (cin, cmd);
		first = stoi(cmd, nullptr, 10);

		cout << "Enter index of second MAC address: ";
		getline (cin, cmd);
		second = stoi(cmd, nullptr, 10);

		cout << "Group name(leave blank for generated): ";
		getline (cin, group);
		

		if(first >= parser.table.size() || second >= parser.table.size())
		{
			cerr << "Invalid index" << endl;
			return 1;
		}
		if(group.empty())
		{
			group.assign(indexs[first] + "-" + indexs[second]);
		}
		// Modify
		parser.table[indexs[first]].group = group;
		parser.table[indexs[second]].group = group;
		printTable(parser.table);

	} while (false);

	parser.dump(out, parser.table);
	out.close();

	return 0;
}

void printTable(ScanTable &table)
{
	int i = 0;
	for(auto &mac: table)
	{
		indexs[i] = mac.first;
		cout << i << ". " << mac.first << "\t group = ";
		if (!mac.second.group.empty())
			cout << "'" << mac.second.group << "'" << endl;
		else
			cout << "None" << endl;
		++i;
	}
}
