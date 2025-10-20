#ifndef MONITORGAMES_H
#define MONITORGAMES_H

#include <memory>
#include <mutex>
#include <unordered_map>

#include "../common/messageDTOs.h"

#include "client_handler.h"

class MonitorClients {
private:
    std::unordered_map<int, std::list<std::shared_ptr<ClientHandler>>> games;
    std::mutex mtx;

public:
    void insert_client_to_race(const int& race_id, std::shared_ptr<ClientHandler> client) {
        std::lock_guard<std::mutex> lock(mtx);
        games[race_id].push_back(std::move(client));
    }

    void remove_race(const int& race_id) {
        std::lock_guard<std::mutex> lock(mtx);
        games.erase(race_id);
    }

    void broadcast_race_state(const int& race_id, const ServerMessageDTO& msg) {
        std::lock_guard<std::mutex> lock(mtx);

        if (!games.contains(race_id)){
            return;
        }

        for (auto& player: games[race_id]) {
            player->send_msg(msg);
        }
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        games.clear();
    }
};
#endif
