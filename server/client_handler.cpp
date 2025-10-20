#include "client_handler.h"

#include <utility>

ClientHandler::ClientHandler(const int id, Socket s,
                             std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue):
        id(id), protocol(std::move(s)), sender(protocol), receiver(protocol, queue, id) {
    receiver.start();
    sender.start();
}

// cppcheck-suppress passedByValue
void ClientHandler::send_msg(const ServerMessageDTO msg) { sender.push_queue(msg); }

bool ClientHandler::is_dead() { return !receiver.is_alive() && !sender.is_alive(); }

int ClientHandler::get_id() const { return id; }
