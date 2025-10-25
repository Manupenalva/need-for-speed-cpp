#ifndef CARSTATE_H
#define CARSTATE_H

#include <cstdint>
#include <memory>

#include "../server/carPhysics.h"

struct CarState {
    uint16_t id;
    float x;
    float y;
    float angle;
    float speed;
    uint16_t lap;
};

struct CarInputState {
    bool accelerating;
    bool braking;
    bool turning_right;
    bool turning_left;
    CarState state;
    std::unique_ptr<CarPhysics> physics;
};

#endif
