#ifndef RACEMONITOR_H
#define RACEMONITOR_H

#include <algorithm>
#include <list>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "../common/constants.h"

#include "client_handler.h"


class RaceStruct {
private:
    std::list<std::shared_ptr<ClientHandler>> players;
    std::mutex mtx;

    void reap();
public:
    RaceStruct();

    bool add_player(std::shared_ptr<ClientHandler> client);

    void remove_player(int id);

    std::vector<int> get_player_ids();

    void set_game_queue(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue);

    void broadcast(const ServerMessageDTO& msg);

    int size();
    
};

#endif
