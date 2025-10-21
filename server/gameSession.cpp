#include "gameSession.h"

GameSession::GameSession(const int& id, MonitorGames& games_monitor):
        games_monitor(games_monitor), user_commands_queue(), gameloop(user_commands_queue, games_monitor, id) {
            games_monitor.set_game_queue(id, user_commands_queue);
        }

void GameSession::start(){
    gameloop.run();
}

void GameSession::stop(){
    gameloop.stop();
    gameloop.join();
}