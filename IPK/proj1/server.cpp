#include <iostream>
#include <cstring>
#include "MyExc.h"
#include "Socket.h"
#include "Packet.h"
#include <stdlib.h>
int main(int argc, char *argv[])
{
    try
    {
        //Packet packet(Opcode::Flags);
        if(argc != 3)
            throw MyExc("Arguments: Wrong count");
        if(strcmp(argv[1],"-p"))
            throw MyExc("Arguments: Parameter -p is required");
        char *err;
        int port = strtol(argv[2],&err, 10);
        if(*err != 0 || port<=0 || port > 65535)
            throw MyExc("Arguments: Second parameter have to be port number");

        std::cout << "Server port: " << port << std::endl;
        Socket socket(port);
        socket.Open();
        socket.Bind();
        socket.Listen();
        socket.Close();
    }
    catch (const MyExc& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
