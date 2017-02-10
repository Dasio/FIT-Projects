#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include "ClientArgParser.h"
#include "MyExc.h"
ClientArgParser::ClientArgParser(int argc,char *argv[]) : m_argv(argv),m_argc(argc),m_flags(0),m_order(0)
{
    if(m_argc < 1 || m_argv == nullptr)
        throw MyExc("ClientArgParser: Wrong input");
}

ClientArgParser::~ClientArgParser() { };

void ClientArgParser::Parse()
{
    if(m_argc < 6)
        throw MyExc("Arguments: Wrong count");
    int ch;
    while ((ch = getopt(m_argc, m_argv, "h:p:luLUGNHS")) != -1)
    {
        switch (ch)
        {
            case 'h':
                SetFlag(HOSTNAME);
                m_hostname = std::string(optarg);
                break;
            case 'p':
                SetFlag(PORT);
                char *err;
                m_port = strtol(optarg,&err, 10);
                if(*err != 0 || m_port <= 0 || m_port > 65535)
                    throw MyExc("Arguments: Invalid port number");
                break;
            case 'l':
                SetFlag(LOGINS);
                while(optind<m_argc && m_argv[optind][0] != '-')
                    m_logins.push_back(m_argv[optind++]);
                break;
            case 'u':
                SetFlag(UIDS);
                while(optind<m_argc && m_argv[optind][0] != '-')
                {
                    char *err;
                    int number = strtol(m_argv[optind++],&err, 10);
                    if(*err != 0 || number < 0)
                        throw MyExc("Arguments: Invalid UID");
                    m_uids.push_back(number);
                }
                break;
            case 'L':
                SetFlag(USER_NAME);
                m_flagsOrder[m_order++] = 0;
                break;
            case 'U':
                SetFlag(UID);
                m_flagsOrder[m_order++] = 1;
                break;
            case 'G':
                SetFlag(GID);
                m_flagsOrder[m_order++] = 2;
                break;
            case 'N':
                SetFlag(NAME);
                m_flagsOrder[m_order++] = 3;
                break;
            case 'H':
                SetFlag(HOME_FOLDER);
                m_flagsOrder[m_order++] = 4;
                break;
            case 'S':
                SetFlag(SHELL);
                m_flagsOrder[m_order++] = 5;
                break;
            default:
                throw MyExc("Arguments: Unknown parameter");
                break;
        }
    }
}
void ClientArgParser::CheckValidity()
{
    if((m_flags & (HOSTNAME | PORT)) != (HOSTNAME | PORT))
        throw MyExc("Arguments: Hostname and port are required");
    /*if(m_flags & LOGINS && m_flags & UIDS)
        throw MyExc("Arguments: Only one of -l -u can be specified");*/
    if(!(m_flags & LOGINS) && !(m_flags & UIDS))
        throw MyExc("Arguments: You need specify at least one of: -l -u");
    if((m_flags & LOGINS && m_logins.empty()) || (m_flags & UIDS && m_uids.empty()))
        throw MyExc("Arguments: You need specify list(at least one thing) in -l -u");
    if(m_flags < USER_NAME)
        throw MyExc("Arguments: You need specify at least one of: -L -U -G -N -H -S");
}
void ClientArgParser::SetFlag(int flag)
{
    if(m_flags & flag)
        throw MyExc("Arguments: Can't enter same argument more times");
    m_flags |= flag;
    if(flag == LOGINS)
        m_flags &= ~UIDS;
    else if(flag == UIDS)
        m_flags &= ~LOGINS;

}
void ClientArgParser::Test()
{
    for(int i = 0; i<m_argc; i++)
        std::cout << m_argv[i] << std::endl;
}
