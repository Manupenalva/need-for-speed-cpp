#include "gameSession.h"

GameSession::GameSession(const int& id, const MonitorGames& players):
        games_monitor(games_monitor), user_commands_queue(), gameloop(user_commands_queue, players) {
            games_monitor.set_game_queue(user_commands_queue);
        }

void GameSession::start(){
    gameloop.run();
}

void GameSession::stop(){
    gameloop.stop();
}