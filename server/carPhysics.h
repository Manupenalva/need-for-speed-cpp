#ifndef CARPHYSICS_H
#define CARPHYSICS_H

#include "../common/carState.h"
#include "../libs/box2d/include/box2d/box2d.h"

class CarPhysics {
private:
    b2BodyId body;
    b2ShapeId shape;
    b2WorldId world;
    CarInfo& car_state;

    float max_speed_factor;
    float acceleration_factor;
    float mass_factor;
    float drivability_factor;

public:
    CarPhysics(b2WorldId world, CarInfo& car_state, const float& max_speed,
               const float& acceleration, const float& mass, const float& drivability,
               const float& car_long, const float& car_width);
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
