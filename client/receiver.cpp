#include "receiver.h"

Receiver::Receiver(Protocol& protocol, Queue<ServerMessageDTO>& server_queue):
        protocol(protocol), server_queue(server_queue) {}

void Receiver::run() {
    try {
        while (_keep_running) {
            ServerMessageDTO msg = protocol.recv_server_message();
            server_queue.push(msg);
        }
    } catch (const std::exception& e) {
        stop();
    }
}

void Receiver::stop() {
    // La cola se destruye en el client, que es quien la crea.
    _keep_running = false;
    _is_alive = false;
}
