#include <exception>

#ifndef MYEXC_H
#define MYEXC_H

class MyExc: public std::exception
{
public:
    MyExc(const std::string &message) : m_msg(message) { }

    virtual const char* what() const throw()
    {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};

#endif // !MYEXC_H
