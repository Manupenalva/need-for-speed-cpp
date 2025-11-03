#ifndef CARPHYSICS_H
#define CARPHYSICS_H

#include "../common/carState.h"
#include "../libs/box2d/include/box2d/box2d.h"

class CarPhysics {
private:
    b2BodyId body;
    b2ShapeId shape;
    b2WorldId world;
    float life;
    CarInfo& car_state;

public:
    CarPhysics(b2WorldId world, CarInfo& car_state);
    void accelerate();
    void brake();
    void turn_left();
    void turn_right();
    void update_position();
    void handle_hits();

private:
    void handle_hit_event(const b2ContactHitEvent& event);
    void handle_crash(const b2Vec2& normal);
};

#endif
