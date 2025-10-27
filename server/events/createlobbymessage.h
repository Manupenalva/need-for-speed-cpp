#ifndef CREATE_LOBBY_MESSAGE_H
#define CREATE_LOBBY_MESSAGE_H

#include <string>

#include "clienthandlermessage.h"

class CreateLobbyMessage: public ClientHandlerMessage {
public:
    explicit CreateLobbyMessage(int client_id):
            ClientHandlerMessage(client_id, MsgType::CREATE_RACE) {}
    virtual ~CreateLobbyMessage() = default;

private:
};

#endif
