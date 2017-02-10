#ifndef PASSWD_H
#define PASSWD_H
#define DELIMETER 10
#define NOT_FOUND 11
#define NOT_FOUND_S "@nf"

#include <string>
#include <vector>
#include <pwd.h>


class Passwd
{
public:
    Passwd(unsigned char flags) : m_flags(flags) { }
    // Not used
    Passwd(unsigned char flags,std::vector<unsigned int> &uids) : m_flags(flags),m_uids(uids) { }
    Passwd(unsigned char flags,std::vector<std::string> &logins) : m_flags(flags),m_logins(logins) { }

    void loadRecordsByUIDs();
    void loadRecordsByLogins();

    void loadData(std::vector<unsigned int> uids) { m_uids = uids; loadRecordsByUIDs(); }
    void loadData(std::vector<std::string> logins) { m_logins = logins; loadRecordsByLogins(); }
    const std::vector<std::string> &getRecords() const { return m_records; }

    enum Flags : unsigned char
    {
        LOGIN_OR_UID=0x01,
        USER_NAME = 0x02,
        UID = 0x04,
        GID = 0x08,
        NAME = 0x10,
        HOME_FOLDER = 0x20,
        SHELL = 0x40

    };
private:
    bool my_getpwnam(const char *name);
    bool my_getpwuid(unsigned int uid);
    void AddToRecords(struct passwd *pRecord);
    inline void AddToString(std::string &string,char *record);
    inline void AddToString(std::string &string,unsigned int number);
    unsigned char m_flags;
    std::vector<unsigned int> m_uids;
    std::vector<std::string> m_logins;
    std::vector<std::string> m_records;
};
#endif
