#include "monitorGames.h"

bool MonitorGames::insert_client_to_race(const int& game_id,
                                         std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = games.find(game_id);
    if (it == games.end()) {
        return false;
    }
    return it->second->add_player(client);
}

void MonitorGames::remove_client_from_race(const int& game_id, const int& client_id) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = games.find(game_id);
    if (it != games.end()) {
        it->second->remove_player(client_id);
    }
    if (it->second->size() == 0) {
        games.erase(it);
    }
}


int MonitorGames::create_race() {
    std::lock_guard<std::mutex> lock(mtx);
    games[next_game_id] = std::make_shared<RaceStruct>();
    next_game_id++;
    return next_game_id - 1;
}


void MonitorGames::remove_race(const int& game_id) {
    std::lock_guard<std::mutex> lock(mtx);
    games.erase(game_id);
}

void MonitorGames::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    games.clear();
}

std::shared_ptr<RaceStruct> MonitorGames::get_race(const int& game_id) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = games.find(game_id);
    if (it != games.end()) {
        return it->second;
    }
    return nullptr;
}
