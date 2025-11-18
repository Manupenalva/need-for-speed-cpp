#ifndef CHEAT_CODES_H
#define CHEAT_CODES_H

#include <cstdint>
enum class CheatCode : uint8_t {
    INFINITE_HEALTH = 0,
    WIN_AUTOMATICALLY,
    LOSE_AUTOMATICALLY,
    MAX_STATS,

    INVALID_CHEAT = 255
};

#endif
