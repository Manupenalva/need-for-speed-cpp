#ifndef MONITORGAMES_H
#define MONITORGAMES_H

#include <memory>
#include <mutex>
#include <unordered_map>

#include "../common/messageDTOs.h"

#include "client_handler.h"

class MonitorGames {
private:
    std::unordered_map<int, std::list<std::shared_ptr<ClientHandler>>> games;
    std::mutex mtx;

public:
    void insert_client_to_race(const int& game_id, std::shared_ptr<ClientHandler> client) {
        std::lock_guard<std::mutex> lock(mtx);
        games[game_id].push_back(client);
    }

    std::vector<int> get_players_id(const int& game_id){
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<int> players_id;

        if (!games.contains(game_id)){
            return players_id;
        }

        for (auto& player: games[game_id]) {
            players_id.push_back(player->get_id());
        }

        return players_id;
    }

    void set_game_queue(const int& game_id, std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> game_queue){
        std::lock_guard<std::mutex> lock(mtx);

        if (!games.contains(game_id)){
            return;
        }

        for (auto& player: games[game_id]) {
            player->set_game_queue(game_queue);
        }
    }

    void remove_race(const int& game_id) {
        std::lock_guard<std::mutex> lock(mtx);
        games.erase(game_id);
    }

    void broadcast_race_state(const int& game_id, const ServerMessageDTO& msg) {
        std::lock_guard<std::mutex> lock(mtx);

        if (!games.contains(game_id)){
            return;
        }

        for (auto& player: games[game_id]) {
            player->send_msg(msg);
        }
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        games.clear();
    }
};
#endif
