#ifndef MSGTYPE_H
#define MSGTYPE_H

#include <cstdint>
enum class MsgType : uint8_t {
    END_CONNECTION = 0,
    // Cliente → Servidor
    DRIVING_EVENT = 1,
    JOIN_RACE,
    CREATE_RACE,
    EXIT_RACE,
    GET_LOBBIES,
    START_RACE,
    GET_LOBBY_UPDATE,
    GET_CAR_CATALOG,
    SELECT_CAR,
    SET_READY,
    CHEAT_CODE,

    // Servidor → Cliente
    GAME_START,
    GAME_END,
    STATE_UPDATE,
    SEND_LOBBIES_INFO,
    JOIN_RESULT,
    SEND_CLIENT_ID,
    SEND_LOBBY_UPDATE,
    RACE_FINISHED,
    RACE_STARTED,
    INTERVAL_UPDATE,
    SEND_CAR_CATALOG,
    SEND_MAP_NUMBER,
    SEND_MINIMAP_INFO,
    INTERVAL_CLOSED,
    RACE_POSITIONS,
    ACCUMULATED_POSITIONS,

    INVALID = 255

};
#endif
