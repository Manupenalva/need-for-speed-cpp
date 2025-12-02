#include "monitorClients.h"

void MonitorClients::add_client(int id, std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    clients[id] = client;
}

void MonitorClients::remove(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    client_names.erase(clients[id]->get_username());
    clients.erase(id);
}

void MonitorClients::remove_if_dead() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto it = clients.begin(); it != clients.end();) {
        if (it->second->is_dead()) {
            client_names.erase(it->second->get_username());
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
    client_names.clear();
    clients.clear();
}

bool MonitorClients::change_username(int id, const std::string& new_username) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = clients.find(id);
    if (it == clients.end()) {
        return false;
    }
    if (client_names.find(new_username) != client_names.end()) {
        return false;
    }

    std::string old_username = it->second->get_username();
    client_names.erase(old_username);
    it->second->set_username(new_username);
    client_names.insert(new_username);

    return true;
}
