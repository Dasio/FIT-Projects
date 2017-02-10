#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <set>
#include "connection.h"

class ConnectionManager
{
public:
    ConnectionManager() { };
    void start(connection_ptr c);
    void stop(connection_ptr c);
    void stop_all();
private:
    std::set<connection_ptr> connections;
};
#endif /* !CONNECTION_MANAGER_H */
