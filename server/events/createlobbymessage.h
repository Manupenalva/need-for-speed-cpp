#ifndef CREATE_LOBBY_MESSAGE_H
#define CREATE_LOBBY_MESSAGE_H

#include <string>

#include "clienthandlermessage.h"

class CreateLobbyMessage: public ClientHandlerMessage {
public:
    CreateLobbyMessage(int client_id, const std::string& lobby_name):
            ClientHandlerMessage(client_id), lobby_name(lobby_name) {}
    virtual ~CreateLobbyMessage() = default;

    std::string get_lobby_name() const { return lobby_name; }

private:
    std::string lobby_name;
};

#endif
