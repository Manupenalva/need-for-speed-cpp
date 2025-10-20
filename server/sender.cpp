#include "sender.h"

Sender::Sender(Protocol& protocol): protocol(protocol) {}

void Sender::run() {
    while (should_keep_running() && protocol.socket_alive()) {

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
