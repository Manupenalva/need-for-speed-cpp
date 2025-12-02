#ifndef CARPHYSICS_H
#define CARPHYSICS_H

#include "../../common/carState.h"
#include "../../libs/box2d/include/box2d/box2d.h"

#include "carConstants.h"

class Car;

class CarPhysics {
private:
    CarConstants::Physics& phy_const;
    b2BodyId body;
    b2ShapeId shape;
    b2WorldId world;
    CarInfo& car_state;

    float max_speed_factor;
    float acceleration_factor;
    float mass_factor;
    float drivability_factor;
    float nitro_remaining;

public:
    CarPhysics(b2WorldId world, CarInfo& car_state, const float& max_speed,
               const float& acceleration, const float& mass, const float& drivability,
               const float& car_long, const float& car_width, Car* car,
               CarConstants::Physics& physic_constants);
    void accelerate();
    void deaccelerate();
    void brake();
    void turn_left();
    void turn_right();
    void handle_nitro();
    void update_position();
    void handle_hits();
    void handle_crash_with_bridge();
    void set_stats(const float& max_speed, const float& acceleration, const float& mass,
                   const float& drivability);

private:
    void handle_hit_event(const b2ContactHitEvent& event);
    void handle_crash(const b2Vec2& normal, const float impact_force);
    void apply_damage(const int dmg);
    void handle_crash_damage(const float impact_force, const float crash_direction);
    float get_mass_from_shape(b2ShapeId shapeId);
};

#endif
