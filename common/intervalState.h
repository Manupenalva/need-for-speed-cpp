#ifndef INTERVALSTATE_H
#define INTERVALSTATE_H

#include <cstdint>
#include <vector>

#include "playerState.h"

struct IntervalState {
    uint8_t players_ready;
    uint8_t total_players;
    std::vector<PlayerState> player_states;
};

#endif
