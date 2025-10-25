#ifndef CARPHYSICS_H
#define CARPHYSICS_H

#include "../libs/box2d/include/box2d/box2d.h"
#include "../libs/box2d/include/box2d/id.h"

class CarPhysics {
private:
    b2BodyId body;
    b2WorldId world;
    bool is_colliding;

public:
    CarPhysics(b2WorldId world, float x, float y);
    void accelerate();
    void brake();
    void turn_left();
    void turn_right();

    float get_x_position();
    float get_y_position();
    float get_angle();
    float get_speed();
};

#endif
