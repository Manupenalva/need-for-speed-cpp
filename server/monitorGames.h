#ifndef MONITORGAMES_H
#define MONITORGAMES_H

#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../common/messageDTOs.h"

#include "client_handler.h"

class MonitorGames {
private:
    std::unordered_map<int, std::list<std::shared_ptr<ClientHandler>>> games;
    std::mutex mtx;

public:
    void insert_client_to_race(const int& game_id, std::shared_ptr<ClientHandler> client);
    std::vector<int> get_players_id(const int& game_id);
    void set_game_queue(const int& game_id,
                        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> game_queue);
    void remove_race(const int& game_id);
    void broadcast_race_state(const int& game_id, const ServerMessageDTO& msg);
    void clear();
};

#endif
