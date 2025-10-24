#ifndef CARPHYSICS_H
#define CARPHYSICS_H

#include <box2d/box2d.h>

class CarPhysics {
private:
    b2Body* body;
    bool is_colliding;

public:
    CarPhysics(b2World& world, float x, float y);
    void accelerate();
    void brake();
    void turn_left();
    void turn_right();

    float get_x_position();
    float get_y_position();
    float get_angle();
    float get_speed();

private:
    GameSession(const GameSession& other) = delete;
    GameSession& operator=(const GameSession& other) = delete;
};











#endif
