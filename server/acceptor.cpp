#include "acceptor.h"

#include <memory>
#include <utility>

Acceptor::Acceptor(Socket& acceptor, Queue<std::shared_ptr<ClientHandlerMessage>>& queue,
                   MonitorClients& clients):
        acceptor(acceptor), queue(queue), id(0), clients(clients) {}

void Acceptor::run() {
    while (!acceptor.is_stream_recv_closed()) {
        reap();
        std::shared_ptr<ClientHandler> client;
        try {
            Socket socket_com = acceptor.accept();
            client = std::make_shared<ClientHandler>(id, std::move(socket_com), queue);
        } catch (const std::exception& e) {
            break;  // Aceptador sale de bucle y limpia todos los client handlers
        }
        clients.insert(id, client);
        id++;
    }
    clear();
}


void Acceptor::reap() { clients.remove_if_dead(); }

void Acceptor::clear() { clients.clear(); }

void Acceptor::shutdown() {
    acceptor.shutdown(2);
    acceptor.close();
}

Acceptor::~Acceptor() {
    clear();
    this->join();
}
