#include "gameSession.h"

#include <iostream>

GameSession::GameSession(const int& id, MonitorClients& clients_monitor):
        clients_monitor(clients_monitor),
        game_id(id),
        user_commands_queue(std::make_shared<Queue<std::shared_ptr<ClientHandlerMessage>>>()),
        gameloop(user_commands_queue, clients_monitor, id) {
    clients_monitor.set_game_queue(id, user_commands_queue);
    gameloop.start();
}

// void GameSession::start() { gameloop.start(); }

void GameSession::stop() {
    gameloop.stop();
    gameloop.join();
}

GameSession::~GameSession() {
    stop();
    clients_monitor.remove_race(game_id);
}

bool GameSession::is_running() const { return gameloop.is_alive(); }
