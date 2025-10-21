#include "server.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

Server::Server(const std::string& servname):
        acceptor(servname, lobby_queue, clients_monitor, games_monitor) {}


void Server::communicate_to_client() {
    bool keep_reading = true;

    while (keep_reading) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string command;

        iss >> command;
        if (command == "q") {
            keep_reading = false;
        } else if (command == "start") {
            start_race();
        }
    }
}

void Server::start_race() {
    std::unique_ptr<GameSession> game_session = std::make_unique<GameSession>(0, games_monitor);
    // Idem acceptor, OJO con esto, es responsabilidad de lobby

    game_session->start();

    games.push_back(std::move(game_session));
}

void Server::start() {
    acceptor.start();
    communicate_to_client();
}

void Server::shutdown() {
    acceptor.stop();
    acceptor.join();

    for (auto& game: games) {
        game->stop();
    }

    lobby_queue->close();
}
