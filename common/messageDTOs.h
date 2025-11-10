#ifndef MESSAGE_DTO_H
#define MESSAGE_DTO_H

#include <string>
#include <vector>

#include "carProperties.h"
#include "intervalState.h"
#include "lobbyinfo.h"
#include "msgType.h"
#include "state.h"

// Mensaje que manda el cliente al servidor
struct ClientMessageDTO {
    MsgType type;
    std::vector<uint8_t> events;
    uint16_t lobby_id;
    uint16_t car_id;

    ClientMessageDTO(): type(MsgType::INVALID), events(), lobby_id(-1), car_id(0) {}
};


// Mensaje que manda el servidor al cliente
struct ServerMessageDTO {
    MsgType type;
    State state;
    std::string message;
    LobbyInfo lobby_info;
    bool joined = false;
    int id;
    uint8_t map_number;
    std::vector<CarProperties> car_catalog;
    IntervalState interval_state;

    ServerMessageDTO():
            type(MsgType::INVALID),
            state(),
            message(""),
            lobby_info(),
            joined(false),
            id(0),
            map_number(0),
            car_catalog(),
            interval_state() {}
};
#endif
