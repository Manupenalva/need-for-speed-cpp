#ifndef JOIN_LOBBY_MESSAGE_H
#define JOIN_LOBBY_MESSAGE_H

#include <string>

#include "clienthandlermessage.h"

class JoinLobbyMessage: public ClientHandlerMessage {
public:
    JoinLobbyMessage(int client_id, int race_id):
            ClientHandlerMessage(client_id, MsgType::JOIN_RACE), race_id(race_id) {}
    virtual ~JoinLobbyMessage() = default;
    int get_race_id() override { return race_id; }

private:
    int race_id;
};

#endif
