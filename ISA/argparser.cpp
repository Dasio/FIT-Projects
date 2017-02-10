#include "argparser.h"
#include "string.h"
#include "unistd.h"
#include "ISAExc.h"
#include <iostream>
ArgParser::ArgParser(int argc, char *argv[])
 : _argc{argc},_argv{argv}, _port{0}, _maxClients{maxClients}, _maxAttempts{maxAttempts}
{
    if (_argc<2 || _argv == nullptr)
        throw ISAExc("ArgParser: Not enough parameters");
}

void ArgParser::parse()
{
    int ch;
    char *err;
    // Basic parsing by getopt
    while ((ch = getopt(_argc,_argv, "m:a:p:l:r:c:t:")) != -1)
    {
        switch (ch)
        {
            case 'm':
                if(!strcmp(optarg,"ftp"))
                    _mode = FTP;
                else if(!strcmp(optarg, "ssh"))
                    _mode = SSH;
                else
                    throw ISAExc("ArgParser: Invalid argument for -m, only ftp and ssh are supported");
                break;
            case 'a':
                _address = std::string(optarg);
                break;
            case 'l':
                _logFile = std::string(optarg);
                break;
            case 'p':
                _port = strtol(optarg,&err,10);
                if(*err != 0 || _port <= 0 || _port > 65535)
                    throw ISAExc("ArgParser: Invalid port number");
                break;
            case 'r':
                _rsaKey = std::string(optarg);
                break;
            case 'c':
                _maxClients = strtol(optarg,&err,10);
                if(*err != 0 || _maxClients < 1)
                    throw ISAExc("ArgParser: Invalid number for max clients -c");
                break;
            case 't':
                _maxAttempts = strtol(optarg,&err,10);
                if(*err != 0 || _maxAttempts < 1)
                    throw ISAExc("ArgParser: Invalid number for max attemps, at least one is required");
                break;
            default:
                throw ISAExc("ArgParser: Unknown parameter");
                break;
        }
    }

    // Additional checks
    if (_mode == NONE)
        throw ISAExc("ArgParser: Mode is required, -m <ftp/ssh>");
    else if(_address.size() == 0)
        throw ISAExc("ArgParser: Address is required, -a <address>");
    else if (_port == 0)
        throw ISAExc("ArgParser: Port number is required, -p <port>");
    else if (_logFile.size() == 0)
        throw ISAExc("ArgParser: Logfile is required, -l <logfile>");
    else if (_mode == SSH && _rsaKey.size() == 0)
        throw ISAExc("ArgParser: In SSH mode is required to specify rsa-key, -r <path to key>");
    if (_mode == FTP && _rsaKey.size() != 0)
        std::cerr << "ArgParser: Ignoring -r, not required in FTP mode" << std::endl;

}

