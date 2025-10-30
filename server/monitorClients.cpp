#include "monitorClients.h"

void MonitorClients::add_client(int id, std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    clients[id] = client;
}

void MonitorClients::remove(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(id);
}

void MonitorClients::remove_if_dead() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto it = clients.begin(); it != clients.end();) {
        if (it->second->is_dead()) {
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}

std::shared_ptr<ClientHandler> MonitorClients::get_client(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = clients.find(id);
    return (it == clients.end()) ? nullptr : it->second;
}

void MonitorClients::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    clients.clear();
}
