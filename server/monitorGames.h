#ifndef MONITORGAMES_H
#define MONITORGAMES_H

#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../common/messageDTOs.h"

#include "client_handler.h"
#include "racemonitor.h"

class MonitorGames {
private:
    std::unordered_map<int, std::shared_ptr<Race>> games;
    std::mutex mtx;
    int next_game_id = 1;

public:
    int create_race();
    bool insert_client_to_race(const int& game_id, std::shared_ptr<ClientHandler> client);
    void remove_race(const int& game_id);
    void clear();
    void remove_client_from_race(const int& game_id, const int& client_id);
    std::shared_ptr<Race> get_race(const int& game_id);
};

#endif
