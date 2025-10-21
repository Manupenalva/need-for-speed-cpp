#include "gameSession.h"

#include <iostream>

GameSession::GameSession(const int& id, MonitorGames& games_monitor):
        games_monitor(games_monitor),
        game_id(id),
        user_commands_queue(std::make_shared<Queue<std::shared_ptr<ClientHandlerMessage>>>()),
        gameloop(user_commands_queue, games_monitor, id) {
    games_monitor.set_game_queue(id, user_commands_queue);
}

void GameSession::start() { gameloop.start(); }

void GameSession::stop() {
    gameloop.stop();
    gameloop.join();
}
