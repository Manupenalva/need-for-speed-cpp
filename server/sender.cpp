#include "sender.h"

#include <utility>

Sender::Sender(Protocol& protocol, int id): protocol(protocol), id(id) { this->start(); }

void Sender::run() {
    send_client_id();
    while (should_keep_running() && protocol.socket_alive()) {

        try {
            ServerMessageDTO msg = queue.pop();
            protocol.send_server_message(msg);
        } catch (const ClosedQueue& e) {
            return;
        } catch (const std::exception& e) {
            std::cerr << "Sender error: " << e.what() << std::endl;
            stop();
        }
    }
}

void Sender::kill() {
    if (!should_keep_running()) {
        return;
    }
    stop();
    queue.close();
}


void Sender::push_queue(ServerMessageDTO msg) { queue.try_push(std::move(msg)); }

Sender::~Sender() {
    kill();
    join();
}

void Sender::send_client_id() {
    ServerMessageDTO msg;
    msg.type = MsgType::SEND_CLIENT_ID;
    msg.id = id;
    protocol.send_server_message(msg);
}
