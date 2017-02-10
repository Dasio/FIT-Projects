#include <iostream>
#include "server.h"
#include <boost/program_options.hpp>
int main(int argc,char **argv)
{
    int port,t,maxc;
    namespace po = boost::program_options;
    po::options_description description("Options");
    description.add_options()
        ("help,h","Print help message")
        ("chunk-max-size,c,",po::value<int>(&maxc)->default_value(0),"Set max size of data in chunk")
        ("port,p",po::value<int>(&port)->default_value(80),"Set port for listening")
        ("min-chunk-time,t",po::value<int>(&t)->default_value(0),"Set minimal time before sending next chunk");
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, description),vm);
        if (vm.count("help"))
        {
            std::cout << description << std::endl;
            return 0;
        }
        po::notify(vm);
    }
    catch(po::error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    Server server(port,maxc,t);
    server.run();
    return 0;
}
