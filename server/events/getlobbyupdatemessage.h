#ifndef GETLOBBYUPDATEMESSAGE_H
#define GETLOBBYUPDATEMESSAGE_H

#include "clienthandlermessage.h"

class GetLobbyUpdateMessage : public ClientHandlerMessage {

public:
    explicit GetLobbyUpdateMessage(int client_id):
            ClientHandlerMessage(client_id, MsgType::GET_LOBBY_UPDATE) {}
};

#endif
    