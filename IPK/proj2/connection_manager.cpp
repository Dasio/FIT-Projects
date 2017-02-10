#include "connection_manager.h"

void ConnectionManager::start(connection_ptr c)
{
    connections.insert(c);
    c->start();
}

void ConnectionManager::stop(connection_ptr c)
{
    connections.erase(c);
    c->stop();
}

void ConnectionManager::stop_all()
{
    for (auto c: connections)
        c->stop();
    connections.clear();
}
