#ifndef JOIN_LOBBY_MESSAGE_H
#define JOIN_LOBBY_MESSAGE_H

#include "clienthandlermessage.h"
#include <string>

class JoinLobbyMessage : public ClientHandlerMessage {
public:
    JoinLobbyMessage(int client_id, const std::string& lobby_name)
        : ClientHandlerMessage(client_id), lobby_name(lobby_name) {}
    virtual ~JoinLobbyMessage() = default;
    std::string get_lobby_name() const { return lobby_name; }
private:
    std::string lobby_name;
};

#endif
