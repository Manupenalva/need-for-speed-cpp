#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <memory>

#include "gameloop.h"
#include "monitorclients.h"

class GameSession {
private:
    MonitorClients& clients_monitor;
    int game_id;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue;
    Gameloop gameloop;

public:
    GameSession(const int& id, MonitorClients& clients_monitor);
    // void start();
    void stop();
    ~GameSession();

    bool is_running() const;

private:
    GameSession(const GameSession& other) = delete;
    GameSession& operator=(const GameSession& other) = delete;
};

#endif
