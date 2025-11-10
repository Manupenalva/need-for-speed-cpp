#include "racemonitor.h"

RaceStruct::RaceStruct(): players(), mtx() {}

bool RaceStruct::add_player(std::shared_ptr<ClientHandler> client) {
    std::lock_guard<std::mutex> lock(mtx);
    reap();
    if (players.size() >= MAX_PLAYERS_RACE) {
        return false;
    }
    players.push_back(std::move(client));
    return true;
}

void RaceStruct::remove_player(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    players.remove_if([&](auto& c) { return c->get_id() == id; });
}

std::vector<int> RaceStruct::get_player_ids() {
    std::lock_guard<std::mutex> lock(mtx);
    reap();
    std::vector<int> ids;
    ids.reserve(players.size());
    std::transform(players.begin(), players.end(), std::back_inserter(ids),
                   [](const std::shared_ptr<ClientHandler>& p) { return p->get_id(); });
    return ids;
}

void RaceStruct::broadcast(const ServerMessageDTO& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    reap();
    for (auto& client: players) {
        client->send_msg(msg);
    }
}

void RaceStruct::set_game_queue(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue) {
    std::lock_guard<std::mutex> lock(mtx);
    reap();
    for (auto& client: players) {
        client->set_game_queue(new_queue);
    }
}

int RaceStruct::size() {
    std::lock_guard<std::mutex> lock(mtx);
    reap();
    return players.size();
}

void RaceStruct::reap() {
    players.remove_if([](auto& c) { return c->is_dead(); });
}
