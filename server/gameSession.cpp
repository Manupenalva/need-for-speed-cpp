#include "gameSession.h"

#include <iostream>

GameSession::GameSession(const int& id, std::shared_ptr<RaceStruct> race,
                         std::shared_ptr<ConfigConstants> config_constants):
        race(race),
        game_id(id),
        user_commands_queue(std::make_shared<Queue<std::shared_ptr<ClientHandlerMessage>>>()),
        gameloop(user_commands_queue, race, std::move(config_constants)) {
    race->set_game_queue(user_commands_queue);
    gameloop.start();
}

void GameSession::stop() {
    gameloop.stop();
    gameloop.join();
}

GameSession::~GameSession() { stop(); }

bool GameSession::is_running() const { return gameloop.is_alive(); }

int GameSession::get_id() const { return game_id; }
