#ifndef MESSAGE_DTO_H
#define MESSAGE_DTO_H

#include <string>
#include <vector>

#include "lobbyinfo.h"
#include "msgType.h"
#include "state.h"

// Mensaje que manda el cliente al servidor
struct ClientMessageDTO {
    MsgType type;
    std::vector<uint8_t> events;
    std::string lobby_name;

    ClientMessageDTO(): type(MsgType::INVALID), events(), lobby_name("") {}
};


// Mensaje que manda el servidor al cliente
struct ServerMessageDTO {
    MsgType type;
    State state;
    std::string message;
    std::vector<LobbyInfo> lobbies;
    bool joined = false;
    int id;

    ServerMessageDTO(): type(MsgType::INVALID), state(), message(""), lobbies(), joined(false), id(0) {}
};
#endif
