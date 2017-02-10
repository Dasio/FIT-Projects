#ifndef ARGPARSER_H
#define ARGPARSER_H

#include "string"

class ArgParser
{

public:
    enum Mode
    {
        NONE,
        FTP,
        SSH
    };
    ArgParser(int argc, char *argv[]);
    ~ArgParser() {};

    /**
     * Starts parsing
     */
    void parse();

    Mode getMode() const { return _mode; }
    unsigned int getPort() const { return _port; }
    std::string getAddress() const { return _address; }
    unsigned int getMaxClients() const { return _maxClients; }
    unsigned int getMaxAttempts() const { return _maxAttempts; }
    const char* getRsaKeyFile() const { return _rsaKey.c_str(); }
    std::string getLogFile() const { return _logFile; }

private:
    static const unsigned int maxClients = 10;
    static const unsigned int maxAttempts = 3;
    int _argc;
    char **_argv;
    unsigned int _port;
    unsigned int _maxClients;
    unsigned int _maxAttempts;
    std::string _address;
    std::string _logFile;
    std::string _rsaKey;
    Mode _mode;
};



#endif // !ARGPARSER_H
