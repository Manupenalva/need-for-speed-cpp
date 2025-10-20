#include "sender.h"

Sender::Sender(Protocol& protocol, Queue<ClientMessageDTO>& events_queue)
    : protocol(protocol), events_queue(events_queue) {}

void Sender::run() {
    try {
        while (_keep_running) {
            ClientMessageDTO msg = events_queue.pop();
            protocol.send_client_message(msg);
        }
    } catch (const std::exception& e) {
        stop();
    }
}


void Sender::stop() {
    // La cola se destruye en el client, que es quien la crea.
    _keep_running = false;
    _is_alive = false;
}