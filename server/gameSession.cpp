#include "gameSession.h"

GameSession::GameSession(std::list<std::unique_ptr<ClientHandler>>&& players):
        players(std::move(players)), user_commands_queue(), gameloop(user_commands_queue, players) {
            for(auto& clientHandler: this->players){
                clientHandler->set_queue(&user_commands_queue);
            }
        }

void GameSession::start(){
    gameloop.run();
}

void GameSession::stop(){
    gameloop.stop();
}