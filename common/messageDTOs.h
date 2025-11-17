#ifndef MESSAGE_DTO_H
#define MESSAGE_DTO_H

#include <string>
#include <vector>

#include "carProperties.h"
#include "intervalState.h"
#include "lobbyinfo.h"
#include "msgType.h"
#include "state.h"
#include "cheatcodes.h"
#include "minimapInfo.h"

// Mensaje que manda el cliente al servidor
struct ClientMessageDTO {
    MsgType type;
    std::vector<uint8_t> events;
    uint16_t lobby_id;
    uint16_t car_id;
    CheatCode cheat_code;

    ClientMessageDTO(): type(MsgType::INVALID), events(), lobby_id(-1), car_id(0), cheat_code(CheatCode::INVALID_CHEAT) {}
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
    MinimapInfo minimap_info;
    std::vector<std::pair<uint16_t, float>> positions;

    ServerMessageDTO():
            type(MsgType::INVALID),
            state(),
            message(""),
            lobby_info(),
            joined(false),
            id(0),
            map_number(0),
            car_catalog(),
            interval_state(),
            minimap_info(),
            positions() {}
};
#endif
