#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <memory>
#include <string>

#include "acceptor.h"
#include "gameSession.h"
#include "lobby.h"


class Server {
private:
    MonitorClients clients_monitor;
    MonitorGames games_monitor;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>>
            lobby_queue;  // esto va a pasar a lobby
    Acceptor acceptor;    // Hay que cambiar por la clase Lobby
    std::list<std::unique_ptr<GameSession>>
            games;  // hay que poner una lista protegida de GameSession cuando tengamos varias,
                    // quizas las tiene lobby
    Lobby lobby;

public:
    explicit Server(const std::string& servname);
    void start();
    void shutdown();

private:
    void communicate_to_client();
    void start_race();
    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
};

#endif
