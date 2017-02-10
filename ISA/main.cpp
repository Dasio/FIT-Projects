#include <iostream>
#include "argparser.h"
#include "ISAExc.h"
#include "ftpserver.h"
#include "sshserver.h"
#include <string.h>

// Copy-paste from task
void printHelp()
{
    std::cout << "Spusteni:\n";
    std::cout << "\t ./fakesrv -m mode -a addr -p port -l logfile [-r rsakey] [-c maxclients] [-t maxattempts]\n\n";
    std::cout << "Poradi parametru je libovolne. Popis spoustecich parametru:\n";
    std::cout << "\tPovinny parametr -m definuje rezim serveru: ftp nebo ssh.\n";
    std::cout << "\tPovinny parametr -a slouzi ke specifikovani IP adresy, na ktere bude server naslouchat.\n";
    std::cout << "\tPovinny parametr -p slouzi ke specifikovani TCP portu, na kterem bude server naslouchat.\n";
    std::cout << "\tPovinny parametr -l slouzi ke specifikovani logovaciho souboru.\n";
    std::cout << "\tParametr -r je povinny pouze v SSH rezimu a slouzi ke specifikovani soukromeho RSA klice.\n";
    std::cout << "\tNepovinny parametr -c slouzi ke specifikovani maximalniho poctu soubezne pripojenych klientu. Vychozi hodnota je 10 (maximalni pocet klientu v pripade nezadani parametru -c).\n";
    std::cout << "\tNepovinny parametr -t slouzi v rezimu SSH ke specifikovani maximalniho poctu pokusu o zadani hesla v ramci jednoho pripojeni. Vychozi hodnota je 3 (maximalni pocet pokusu v pripade rezimu SSH a nezadani parametru -t)."
     << std::endl;

}
int main(int argc, char *argv[])
{
    try
    {
      ArgParser parser(argc,argv);
      parser.parse();
      if (parser.getMode() == ArgParser::FTP)
      {
        FTPServer server(parser.getAddress(),parser.getPort(),parser.getMaxClients(),
            parser.getLogFile());
        server.run();
      }
      // SSH
      else
      {
        SSHServer server(parser.getAddress(),parser.getPort(), parser.getMaxClients(),
            parser.getMaxAttempts(), parser.getRsaKeyFile(), parser.getLogFile());
        server.run();
      }

    }
    catch (const ISAExc &ex)
    {
        std::cerr << ex.what() << std::endl;
        if(strncmp(ex.what(), "ArgParser", 9) == 0)
          printHelp();
        else
          perror(0);
        return 1;
    }

}
