#include <iostream>
#include <string>
#include <getopt.h>
#include "MyExc.h"
#include "Socket.h"
#include "Packet.h"
#include "ClientArgParser.h"
#include "Passwd.h"
#include <bitset>
#include <map>

//#include "Passwd.h"

int main(int argc, char *argv[])
{
    try
    {

        ClientArgParser args(argc,argv);
        args.Parse();
        args.CheckValidity();
        int fCount = args.getFlagsCount();
        std::vector<unsigned int> rightOrder;
        std::map<unsigned int,unsigned int> order = args.getFlagsOrder();
        for(int i=0;i<fCount;i++)
        {
            int count = 0;
            for(int j=0;j<fCount;j++)
            {
                if(order[i] > order[j])
                    count++;
            }
            rightOrder.push_back(count);
        }
        Socket socket(args.getHostName(),args.getPort());
        int flags = args.getFlags();
        ClientPacket flagsPacket(Opcode::CMSG_FLAGS);
        ClientPacket dataPacket;
        // Hostname and port flag dont need, also logins, we can check if UIDs is set or not, if not then logins flag is set;

        unsigned int size = 0;
        if(args.checkFlag(ClientArgParser::LOGINS))
        {
            for(auto login: args.getLogins())
                size += login.length() + 1;
            flagsPacket.WriteFlagsData(flags >> 3, size + 1);
            dataPacket.Init(Opcode::CMSG_DATA,size+1);
            for(auto login: args.getLogins())
            {
                dataPacket << login;
            }
        }
        else if(args.checkFlag(ClientArgParser::UIDS))
        {
            size = args.getUIDs().size() * sizeof(unsigned int) + 1;
            flagsPacket.WriteFlagsData(flags >> 3, size);
            dataPacket.Init(Opcode::CMSG_DATA,size);
            for(auto uid: args.getUIDs())
                dataPacket << uid;
        }
        else
            throw MyExc("This should never happen");
        socket.Open();
        socket.Connect();
        socket.Send(flagsPacket);
        socket.Send(dataPacket);
        ServerPacket sizePacket(socket.Recv(4));
        ServerPacket sDataPacket(socket.Recv(sizePacket.getDataSize()));
        socket.Close();
        std::vector<std::string> record = sDataPacket.getRecord();
        int i = 0;
        while(!record.empty())
        {
            if(record.front()[0] == NOT_FOUND)
            {
                if(args.checkFlag(ClientArgParser::LOGINS))
                    std::cerr << "Login \"" << args.getLogin(i) << "\" was not found" << std::endl;
                else if(args.checkFlag(ClientArgParser::UIDS))
                    std::cerr << "UID " << args.getUID(i) << " was not found" << std::endl;
            }
            else
                for(auto x: rightOrder)
                {
                    if(x != rightOrder.back())
                        std::cout << record.at(x) << " ";
                    else
                        std::cout << record.at(x) << std::endl;
                }
            record = sDataPacket.getRecord();
            i++;
        }

    }
    catch (const MyExc& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}
