#include "Passwd.h"
#include <iostream>
#include <sstream>
#include <strings.h>

void Passwd::loadRecordsByLogins()
{
    for(auto login:m_logins)
    {
        if(!my_getpwnam(login.c_str()))
            m_records.push_back(NOT_FOUND_S);
    }
}

void Passwd::loadRecordsByUIDs()
{
    for(auto uid:m_uids)
    {
        if(!my_getpwuid(uid))
            m_records.push_back(NOT_FOUND_S);
    }
}

void Passwd::AddToRecords(struct passwd *pRecord)
{
    std::string record;
    if(m_flags & USER_NAME)
        AddToString(record,pRecord->pw_name);
    if(m_flags & UID)
        AddToString(record,pRecord->pw_uid);
    if(m_flags & GID)
        AddToString(record,pRecord->pw_gid);
    if(m_flags & NAME)
        AddToString(record,pRecord->pw_gecos);
    if(m_flags & HOME_FOLDER)
        AddToString(record,pRecord->pw_dir);
    if(m_flags & SHELL
 )       AddToString(record,pRecord->pw_shell);
    m_records.push_back(record);
}

inline void Passwd::AddToString(std::string &string,char *record)
{
    string += record;
    string += DELIMETER;
}

inline void Passwd::AddToString(std::string &string,unsigned int number)
{
    std::stringstream convert;
    convert << number;
    string += convert.str();
    string += DELIMETER;
}

bool Passwd::my_getpwnam(const char *name)
{
    struct passwd  *ptr;
    bool found = false;
    setpwent();
    while ( (ptr = getpwent()) != NULL)
    {
        if (strcasecmp(name, ptr->pw_name) == 0)
        {
            found = true;
            AddToRecords(ptr);
            break;
        }
    }
    endpwent();
    return found;
}

bool Passwd::my_getpwuid(unsigned int uid)
{
    struct passwd  *ptr;
    bool found = false;
    setpwent();
    while ( (ptr = getpwent()) != NULL)
    {
        if (ptr->pw_uid == uid)
        {
            found = true;
            AddToRecords(ptr);
        }
    }
    endpwent();
    return found;
}
