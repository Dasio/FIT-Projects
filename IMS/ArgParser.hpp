#ifndef ARGPARSER_HPP
#define ARGPARSER_HPP

#include <string>

class ArgParser
{
private:
    int argc;
    char **argv;

public:
    ArgParser(int argc, char *argv[]);
    int parse();
    static void printHelp();
    int lattice;
    int steps;
    std::string fileName;
    std::string dataName;
    int genStep;
    double r_rate;


};

#endif
