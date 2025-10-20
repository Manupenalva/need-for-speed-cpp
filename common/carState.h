#ifndef CARSTATE_H
#define CARSTATE_H

struct CarState {
    uint16_t id;
    float x;
    float y;
    float angle;
    float speed;
    uint16_t lap;
};

struct CarInputState{
    bool accelerating;
    bool breaking;
    bool turning_right;
    bool turning_left;
    CarState state;
}

#endif