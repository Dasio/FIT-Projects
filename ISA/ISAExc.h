#ifndef ISAEXC_H
#define ISAEXC_H

#include <exception>
#include <string>

class ISAExc: public std::exception
{
public:
    ISAExc(const std::string &message) : _msg(message) { }

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

#endif // !ISAEXC_H
