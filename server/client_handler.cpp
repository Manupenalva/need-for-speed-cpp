#include "client_handler.h"

#include <utility>

ClientHandler::ClientHandler(const int id, Socket s,
                             std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue):
        id(id),
        protocol(std::move(s)),
        sender(protocol, id),
        receiver(protocol, queue, id),
        race_id(-1) {}

// cppcheck-suppress passedByValue
void ClientHandler::send_msg(const ServerMessageDTO msg) {
    try {
        sender.push_queue(msg);
    } catch (const ClosedQueue& e) {
        return;
    }
}

void ClientHandler::set_game_queue(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue) {
    receiver.set_queue(new_queue);
}

void ClientHandler::set_race_id(int race_id) { this->race_id = race_id; }
int ClientHandler::get_race_id() const { return race_id; }

bool ClientHandler::is_dead() { return !receiver.is_alive() || !sender.is_alive(); }
