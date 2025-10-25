#ifndef CARPHYSICS_H
#define CARPHYSICS_H

#include "../libs/box2d/include/box2d/box2d.h"
struct CarState;

class CarPhysics {
private:
    b2BodyId body;
    b2WorldId world;
    bool is_colliding;
    CarState& car_state;

public:
    CarPhysics(b2WorldId world, CarState& car_state, float x, float y);
    void accelerate();
    void brake();
    void turn_left();
    void turn_right();
    void update_position();
};

#endif
