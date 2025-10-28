#include "sender.h"

Sender::Sender(Protocol& protocol, int id): protocol(protocol), id(id) { this->start(); }

void Sender::run() {
    // send_client_id();
    while (should_keep_running()) {

        try {
            protocol.send_server_message(queue.pop());
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

// cppcheck-suppress passedByValue
void Sender::push_queue(const ServerMessageDTO msg) { queue.try_push(msg); }

Sender::~Sender() {
    kill();
    join();
}

/*
void Sender::send_client_id() {
    ServerMessageDTO msg;
    msg.type = MsgType::SEND_CLIENT_ID;
    msg.id = id;
    protocol.send_server_message(msg);
}
    */
