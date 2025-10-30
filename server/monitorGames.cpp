#include "monitorGames.h"

void MonitorGames::insert_client_to_race(const int& game_id,
                                         std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    games[game_id]->add_player(client);
}

void MonitorGames::remove_client_from_race(const int& game_id,
                                          const int& client_id) {
    std::lock_guard<std::mutex> lock(mtx);
    games[game_id]->remove_player(client_id);
}

void MonitorGames::create_race() {
    std::lock_guard<std::mutex> lock(mtx);
    games[next_game_id] = std::make_shared<Race>();
    next_game_id++;
}


void MonitorGames::remove_race(const int& game_id) {
    std::lock_guard<std::mutex> lock(mtx);
    games.erase(game_id);
}

void MonitorGames::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    games.clear();
}
