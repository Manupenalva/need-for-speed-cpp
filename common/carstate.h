#ifndef CARSTATE_H
#define CARSTATE_H

#include <cstdint>

struct CarState {
    uint16_t id;
    float x;
    float y;
    float angle;
    float speed;
    uint16_t lap;
};

#endif
