#include "acceptor.h"

#include <memory>
#include <utility>

Acceptor::Acceptor(const std::string& servname,
                   std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue,
                   MonitorClients& clients, MonitorGames& games_monitor):
        acceptor(servname.c_str()), queue(std::move(queue)), id(0), clients(clients), games_monitor(games_monitor) {}

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
        games_monitor.insert_client_to_race(0, client); //OJO!, esto solo para primera entrega, creo una sola carrera de id 0.
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
