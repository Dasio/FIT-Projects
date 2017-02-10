#include "uriparser.h"
#include "common.h"

Uri::Uri(const std::string& uri) : path("/")
{
    std::string decoded_path;
    url_decode(uri,decoded_path);
    int state = s_hostname;
    for(auto c: decoded_path)
    {
        switch(state)
        {
            case s_hostname:
                if(c == '/')
                    state = s_path;
                else if(c == ':')
                    state = s_port;
                else
                    server += c;
                break;
            case s_port:
                if(c == '/')
                    state = s_path;
                else
                    port += c;
                break;
            case s_path:
                path += c;
                break;
        }
    }
    if(port.empty())
        port = "80";
}
