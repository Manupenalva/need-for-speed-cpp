#ifndef STATE_H
#define STATE_H

#include <vector>

#include "carstate.h"

struct State {
    uint32_t frame;
    uint16_t num_cars;
    std::vector<CarState> cars;
};

#endif
