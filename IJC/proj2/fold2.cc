/** @file fold2.cc
 * @brief C++ varianta fold.c
 * Riesenie IJC-DU2, priklad a)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
struct optionsStruct
{
	/* Constructor */
	optionsStruct() :  width(80), c(false) {}
	ifstream fileIn;
    unsigned width;
    bool c;
} ;
void fold2(optionsStruct& options);


int main(int argc,char *argv[])
{
	string filename;
	optionsStruct options;
	int value;
	char *endPtr;
	bool fileFound=false;
	ios::sync_with_stdio(false);
	for (int i=1;i<argc;i++)
	{
		string arg = argv[i];
		if (arg == "-w")
		{
			if (++i>=argc)
			{
				 cerr << "Prepinac 'w' vyzaduje parameter" << endl;
				 return EXIT_FAILURE;
			}
			value=strtol(argv[i],&endPtr,10);
			if (*endPtr!=0)
			{
				cerr << "Prepinac 'w' vyzaduje ciselny parameter" << endl;
				return EXIT_FAILURE;
			}
			options.width=value;					 
		}
		else if (arg == "-c")
		{
			options.c=true;
		}
		/* Akykolvek iny argument je predpokladany ako meno suboru */
		else
		{
			if (fileFound)
			{
				 cerr << "Je mozne spracovat len 1 subor!" << endl;
				 return EXIT_FAILURE;
			}
			filename=string(argv[i]);
			fileFound=true;			
		}
	}
	if (fileFound)
	{
		options.fileIn.open(filename);
		if (!options.fileIn.is_open())
		{
			cerr << "Zadany subor sa nepodarilo otvorit" << endl;
			return EXIT_FAILURE;
		}
	}
	fold2(options);
	if (options.fileIn.is_open()) options.fileIn.close();
	return EXIT_SUCCESS;
}
void fold2(optionsStruct& options)
{
	unsigned current=0;
	unsigned length;
	bool newLine=true;
	string word;
	string line;
	/* Na istream "napojime" cin (stdin) */
	istream *streamInput = &cin;
	if (options.fileIn.is_open())
	{
		/* Ak sme predtym uspesne otvorili subor, tak sa bude citat zo suboru */
		streamInput = &options.fileIn;
	}
	while(getline(*streamInput,line))
	{
		istringstream iss(line);
		/* Riadok je prazdny */
		if (line == "")
		{
			current=0;
			if (!newLine) cout << endl;
			cout << endl;
			newLine=true;
			continue;
		}
		/* Rozdelenie riadku na slova */
		while (iss >> word)
		{
				length=word.length();
				current += length;
				if (options.c && length>options.width) word.erase(options.width);
				/* Zapocitanie nutnej medzery */
				if (!newLine) current++;
				if (current>options.width)
				{
					current=length;
					if (!newLine) cout << endl;
					newLine=true;
				}
				if (!newLine) cout << " ";
				cout << word;
				newLine=false;
		}
		
	}
	/* Textovy subor by mal mat na konci suboru newline */
	if (!newLine) cout << endl;
}
