#ifndef MESSAGE_DTO_H
#define MESSAGE_DTO_H

#include <string>
#include <utility>
#include <vector>

#include "carProperties.h"
#include "cheatcodes.h"
#include "intervalState.h"
#include "lobbyinfo.h"
#include "minimapInfo.h"
#include "msgType.h"
#include "resultInfo.h"
#include "state.h"

// Mensaje que manda el cliente al servidor
struct ClientMessageDTO {
    MsgType type;
    std::vector<uint8_t> events;
    uint16_t lobby_id;
    uint16_t car_id;
    CheatCode cheat_code;
    std::string name;

    ClientMessageDTO():
            type(MsgType::INVALID),
            events(),
            lobby_id(-1),
            car_id(0),
            cheat_code(CheatCode::INVALID_CHEAT),
            name("") {}
};


// Mensaje que manda el servidor al cliente
struct ServerMessageDTO {
    MsgType type;
    State state;
    std::string message;
    LobbyInfo lobby_info;
    bool result = false;
    int id;
    uint8_t map_number;
    std::vector<CarProperties> car_catalog;
    IntervalState interval_state;
    MinimapInfo minimap_info;
    std::vector<ResultInfo> positions;

    ServerMessageDTO():
            type(MsgType::INVALID),
            state(),
            message(""),
            lobby_info(),
            result(false),
            id(0),
            map_number(0),
            car_catalog(),
            interval_state(),
            minimap_info(),
            positions() {}
};
#endif
