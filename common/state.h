#ifndef STATE_H
#define STATE_H

#include <vector>

#include "carState.h"

struct State {
    uint32_t frame;
    uint16_t num_cars;
    std::vector<CarState> cars;

    State(): frame(0), num_cars(0), cars() {}
};

#endif
