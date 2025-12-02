#ifndef RACEMONITOR_H
#define RACEMONITOR_H

#include <algorithm>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../common/constants.h"
#include "../comm_threads/client_handler.h"


class RaceStruct {
private:
    std::list<std::shared_ptr<ClientHandler>> players;
    std::mutex mtx;
    bool started;

    void reap();

public:
    RaceStruct();

    bool add_player(std::shared_ptr<ClientHandler> client);

    void remove_player(int id);

    std::vector<int> get_player_ids();

    void set_game_queue(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue);

    void broadcast(const ServerMessageDTO& msg);

    int size();

    std::unordered_map<int, std::string> get_player_usernames();
};

#endif
