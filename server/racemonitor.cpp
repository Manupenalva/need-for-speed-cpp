#include "racemonitor.h"

Race::Race(): players(), mtx() {}

bool Race::add_player(std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    if (players.size() >= MAX_PLAYERS_RACE) {
        return false;
    }
    players.push_back(std::move(client));
    return true;
}

void Race::remove_player(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    players.remove_if([&](auto& c) { return c->get_id() == id; });
}

std::vector<int> Race::get_player_ids() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> ids;
    ids.reserve(players.size());
    std::transform(players.begin(), players.end(), std::back_inserter(ids),
                   [](const std::shared_ptr<ClientHandler>& p) { return p->get_id(); });
    return ids;
}

void Race::broadcast(const ServerMessageDTO& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& client: players) {
        client->send_msg(msg);
    }
}

void Race::set_game_queue(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& client: players) {
        client->set_game_queue(new_queue);
    }
}

int Race::size() {
    std::lock_guard<std::mutex> lock(mtx);
    return players.size();
}
