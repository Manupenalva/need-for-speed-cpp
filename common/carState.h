#ifndef CARSTATE_H
#define CARSTATE_H

#include <cstdint>
#include <memory>

#include "checkpointArrow.h"
#include "checkpointInfo.h"

struct CarInfo {
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
};

struct Position {
    int x;
    int y;
};

#endif
