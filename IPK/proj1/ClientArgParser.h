#ifndef CLIENT_ARG_PARSER_H
#define CLIENT_ARG_PARSER_H

#include <vector>
#include <map>

class ClientArgParser
{
public:
    ClientArgParser(int argc,char *argv[]);
    ~ClientArgParser();
    const std::string &getHostName() const { return m_hostname; }
    const std::vector<std::string> &getLogins() const { return m_logins; }
    const std::vector<unsigned int> &getUIDs() const { return m_uids; }
    const std::map<unsigned int,unsigned int> &getFlagsOrder() const { return m_flagsOrder;}
    const std::string &getLogin(int i) const { return m_logins.at(i); }
    unsigned int getUID(int i) { return m_uids.at(i); }
    unsigned char getFlagsCount() { return m_order; }
    int getPort() const { return m_port; }
    int getFlags() const { return m_flags; }
    bool checkFlag(unsigned int flag) { return m_flags & flag; }
    void Parse();
    void CheckValidity();
    void Test();
    enum Flags
    {
        // Required
        HOSTNAME = 0x001,
        PORT = 0x002,

        // Just one
        LOGINS = 0x004,
        UIDS = 0x008,

        // At least one required
        USER_NAME = 0x010,
        UID = 0x020,
        GID = 0x040,
        NAME = 0x080,
        HOME_FOLDER = 0x100,
        SHELL = 0x200

    };
private:
    void SetFlag(int flag);
    char **m_argv;
    int m_argc,m_port,m_flags;
    unsigned char m_order;
    std::map<unsigned int,unsigned int> m_flagsOrder;
    std::string m_hostname;
    std::vector<std::string> m_logins;
    std::vector<unsigned int> m_uids;

};

#endif
