#ifndef MONITORCLIENTS_H
#define MONITORCLIENTS_H

#include <algorithm>
#include <memory>
#include <mutex>
#include <unordered_map>


#include "client_handler.h"

class MonitorClients {
private:
    std::unordered_map<int, std::shared_ptr<ClientHandler>> clients;
    std::mutex mtx;

public:
    void add_client(int id, std::shared_ptr<ClientHandler> client);

    void remove(int id);

    void remove_if_dead();

    std::shared_ptr<ClientHandler> get_client(int id);

    void clear();
};
#endif
