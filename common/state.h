#ifndef STATE_H
#define STATE_H

#include <vector>

#include "carState.h"
#include "npcState.h"

struct State {
    int countdown_time;
    uint32_t frame;
    uint16_t num_cars;
    std::vector<CarState> cars;
    std::vector<NpcState> npcs;
    float remaining_time;
    State(): countdown_time(0), frame(0), num_cars(0), cars(), npcs(), remaining_time(0.0f) {}
};

#endif
