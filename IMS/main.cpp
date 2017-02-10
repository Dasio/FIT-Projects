#include <iostream>
#include <sstream>
#include <iomanip>
#include "ArgParser.hpp"
#include "CA.hpp"



int main(int argc, char **argv)
{
    std::stringstream ss;

    // Parse command-line parameters
    ArgParser parser(argc,argv);
    if(parser.parse())
        return 1;
    int digits = std::to_string(parser.steps).size();

    // Construct cellular automat
    CA ca(parser.lattice,parser.r_rate);

    // Generate cells
    ca.generateLattice();

    // Generate BMP before first step
    if(parser.genStep !=0 && !parser.fileName.empty())
    {
        ss << std::setw(digits) << std::setfill('0') << 0 << "_";
        ca.generateBMP(ss.str() + parser.fileName + std::string(".bmp"));
    }

    std::cout << "0.step: ";
    ca.printActualStatistic();
    for(int i=1; i<=parser.steps; i++)
    {
        // Perform step of simulation
        ca.nextStep();
        std::cout << i << ".step: ";
        ca.printActualStatistic();
        // If it's set, generate BMP after step
        if(parser.genStep !=0 && !parser.fileName.empty() && i % parser.genStep == 0)
        {
            ss.str("");
            ss << std::setw(digits) << std::setfill('0') << i << "_";
            ca.generateBMP(ss.str() + parser.fileName + std::string(".bmp"));
        }
    }
    // Generate BMP after end of simulation
    if(!parser.fileName.empty())
        ca.generateBMP(parser.fileName + std::string(".bmp"));

    // Save statistic of each step for every cell to file
    if(!parser.dataName.empty())
        ca.saveData(parser.dataName);

    return 0;
}
