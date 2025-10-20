#ifndef MONITORCLIENTS_H
#define MONITORCLIENTS_H

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../common/messageDTOs.h"

#include "client_handler.h"

class Client;

class MonitorClients {
private:
    std::unordered_map<int, std::shared_ptr<ClientHandler>> clients;
    std::mutex mtx;

public:
    void insert(int id, std::shared_ptr<ClientHandler> client) {
        std::lock_guard<std::mutex> lock(mtx);
        clients[id] = client;
    }

    void remove(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        clients.erase(id);
    }

    void remove_if_dead() {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto it = clients.begin(); it != clients.end();) {
            if (it->second->is_dead()) {
                it = clients.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::shared_ptr<ClientHandler> get_client(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = clients.find(id);
        if (it == clients.end()) {
            throw std::runtime_error("Client not found");
        }
        return it->second;
    }

    void broadcast_state(const ServerMessageDTO& msg) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& [id, client]: clients) {
            client->send_msg(msg);
        }
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        clients.clear();
    }
};
#endif
