#include "server.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

Server::Server(const std::string& servname):
        lobby_queue(std::make_shared<Queue<std::shared_ptr<ClientHandlerMessage>>>()),
        acceptor(servname, lobby_queue, clients_monitor),
        lobby(lobby_queue, clients_monitor) {}


void Server::start() {
    acceptor.start();
    lobby.start();
}

void Server::shutdown() {
    acceptor.shutdown();
    lobby.shutdown();
}
