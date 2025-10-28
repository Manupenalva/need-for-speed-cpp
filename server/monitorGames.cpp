#include "monitorGames.h"

void MonitorGames::insert_client_to_race(const int& game_id,
                                         std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    games[game_id].push_back(client);
}

std::vector<int> MonitorGames::get_players_id(const int& game_id) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> players_id;

    if (!games.contains(game_id)) {
        return players_id;
    }

    for (const auto& player: games[game_id]) {
        uint16_t id = player->get_id();
        players_id.push_back(id);
    }

    return players_id;
}

void MonitorGames::set_game_queue(
        const int& game_id,
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> game_queue) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!games.contains(game_id)) {
        return;
    }

    for (auto& player: games[game_id]) {
        player->set_game_queue(game_queue);
    }
}

void MonitorGames::remove_race(const int& game_id) {
    std::lock_guard<std::mutex> lock(mtx);
    games.erase(game_id);
}

void MonitorGames::broadcast_race_state(const int& game_id, const ServerMessageDTO& msg) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!games.contains(game_id)) {
        return;
    }

    for (auto& player: games[game_id]) {
        player->send_msg(msg);
    }
}

void MonitorGames::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    games.clear();
}
