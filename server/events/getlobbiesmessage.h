#ifndef GET_LOBBIES_MESSAGE_H
#define GET_LOBBIES_MESSAGE_H

#include "clienthandlermessage.h"

class GetLobbiesMessage : public ClientHandlerMessage {
public:
    GetLobbiesMessage(int client_id) : ClientHandlerMessage(client_id) {}
    virtual ~GetLobbiesMessage() = default;
};

#endif
