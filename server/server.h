#ifndef SERVER_H
#define SERVER_H

#include <string>

#include "acceptor.h"
#include "gameSession.h"
#include "protectedClientList.h"


class Server {
private:
    Acceptor acceptor; //Hay que cambiar por la clase Lobby
    GameSession gameSession;

public:
    explicit Server(const std::string& servname);
    void start();
    void shutdown();

private:
    void communicate_to_client();

    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
};

#endif
