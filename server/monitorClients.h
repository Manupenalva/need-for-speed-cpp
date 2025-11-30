#ifndef MONITORCLIENTS_H
#define MONITORCLIENTS_H

#include <algorithm>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>

#include "client_handler.h"

class MonitorClients {
private:
    std::unordered_map<int, std::shared_ptr<ClientHandler>> clients;
    std::set<std::string> client_names;
    std::mutex mtx;

public:
    void add_client(int id, std::shared_ptr<ClientHandler> client);

    void remove(int id);

    void remove_if_dead();

    std::shared_ptr<ClientHandler> get_client(int id);

    bool change_username(int id, const std::string& new_username);

    void clear();
};
#endif
