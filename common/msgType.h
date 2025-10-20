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

    // Servidor → Cliente
    GAME_START,
    GAME_END,
    STATE_UPDATE,
    SEND_LOBBIES_INFO,
    JOIN_RESULT,


    INVALID = 255

};
#endif
