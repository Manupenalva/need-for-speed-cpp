#ifndef NPCSTATE_H
#define NPCSTATE_H

#include <cstdint>

struct NpcState {
    float x;
    float y;
    float angle;
    uint16_t car_type;
    bool under_bridge;

    NpcState(float x, float y, float angle, uint16_t car_type, bool under_bridge)
        : x(x), y(y), angle(angle), car_type(car_type), under_bridge(under_bridge) {}
    
    NpcState() : x(0.0f), y(0.0f), angle(0.0f), car_type(1), under_bridge(false) {}
};
#endif
