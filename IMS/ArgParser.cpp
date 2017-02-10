#include <iostream>
#include "ArgParser.hpp"
#include <unistd.h>

ArgParser::ArgParser(int argc, char *argv[]) : argc(argc),argv(argv), lattice(1000), steps(1), genStep(0),r_rate(0.05)
{

}
void ArgParser::printHelp()
{
    std::cout << "IMS 2015/2016" << std::endl;
    std::cout << "\t -h for this help\n";
    std::cout << "\t -l <number> set lattice size l*l, default=1000, minimum=3\n";
    std::cout << "\t -s <number> steps , default=minimum=1\n";
    std::cout << "\t -f <filename> for BMP picture\n";
    std::cout << "\t -d <filename> for generating data\n";
    std::cout << "\t -g <number> generate BMP ficture in each <number> step, -f have to be specified" << std::endl;
}
int ArgParser::parse()
{
    int value;
    double d_value;
    int ch;
    char *err;
    while ((ch = getopt(argc,argv, "hl:s:f:d:g:r:")) != -1)
    {
        switch(ch)
        {
            case 'h':
                printHelp();
                return 1;
            case 'l':
                value = strtol(optarg,&err,10);
                if(*err == 0 && value >= 3)
                    lattice = value;
                break;
            case 's':
                value = strtol(optarg,&err,10);
                if(*err == 0 && value > 0)
                    steps = value;
                break;
            case 'g':
                value = strtol(optarg,&err,10);
                if(*err == 0 && value > 0)
                    genStep = value;
                break;
            case 'f':
                fileName = std::string(optarg);
                break;
            case 'd':
                dataName = std::string(optarg);
                break;
            case 'r':
                d_value = strtod(optarg,&err);
                if(*err == 0 && d_value >= 0.0 && d_value <= 1.0)
                    r_rate = d_value;
                break;
        }
    }
    return 0;
}

