#ifndef URIPARSER_H
#define URIPARSER_H

#include <string>

class Uri
{
public:
    Uri(const std::string& uri);
    std::string server;
    std::string port;
    std::string path;
private:
    enum state
    {
        s_hostname = 0,
        s_port,
        s_path
    };
};
#endif
