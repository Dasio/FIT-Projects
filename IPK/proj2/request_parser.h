#ifndef REQUEST_PARSER
#define REQUEST_PARSER

#include <string>
#include <array>
#include "reply.h"

class RequestParser
{
public:
    RequestParser(std::array<char,8192> _data) : keepAlive(false),readpos(0),data(std::begin(_data),std::end(_data)) { };
    void parse();
    reply handle();
    static std::string gen_list(std::string path);
    std::string method;
    std::string path;
    bool keepAlive;
    static std::string ext2mime(std::string ext);
    static const std::string doc;
private:

    static const std::string img_doc;
    static const std::string img_dir;
    static const std::string img_img;

    int readpos;
    std::string data;

};
#endif
