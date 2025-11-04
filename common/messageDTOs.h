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
    uint16_t lobby_id;

    ClientMessageDTO(): type(MsgType::INVALID), events(), lobby_id(-1) {}
};


// Mensaje que manda el servidor al cliente
struct ServerMessageDTO {
    MsgType type;
    State state;
    std::string message;
    LobbyInfo lobby_info;
    bool joined = false;
    int id;

    ServerMessageDTO():
            type(MsgType::INVALID), state(), message(""), lobby_info(), joined(false), id(0) {}
};
#endif
