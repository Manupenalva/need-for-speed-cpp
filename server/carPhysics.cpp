#include "carPhysics.h"

#include <cmath>
#include <iostream>

#include "car.h"


CarPhysics::CarPhysics(b2WorldId world, CarInfo& car_state, const float& max_speed,
                       const float& acceleration, const float& mass, const float& drivability,
                       const float& car_long, const float& car_width, Car* car,
                       CarConstants::Physics& physic_constants):
        phy_const(physic_constants),
        world(world),
        car_state(car_state),
        max_speed_factor(max_speed / 100.0f),
        acceleration_factor(acceleration / 100.0f),
        mass_factor(mass / 100.0f),
        drivability_factor(drivability / 100.0f),
        nitro_remaining(100.0f) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = phy_const.BASE_FRICTION * mass_factor;
    bodyDef.angularDamping = phy_const.BASE_FRICTION * mass_factor;
    bodyDef.position = {car_state.x, car_state.y};
    body = b2CreateBody(world, &bodyDef);
    b2Body_EnableContactEvents(body, true);
    b2Body_EnableHitEvents(body, true);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Polygon box = b2MakeBox((car_long / 2.0f), (car_width / 2.0f));
    shape = b2CreatePolygonShape(body, &shapeDef, &box);
    b2Shape_EnableSensorEvents(shape, true);
    b2Shape_EnableContactEvents(shape, true);
    b2Shape_EnableHitEvents(shape, true);
    b2Body_SetUserData(body, car);
}

void CarPhysics::accelerate() {
    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 direction = {cosf(rad), sinf(rad)};

    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float max_speed = phy_const.BASE_MAX_SPEED * max_speed_factor;
    float speedRatio = 0.0f;
    if (speed >= phy_const.MIN_SPEED) {
        speedRatio = max_speed / speed;
    }
    float health_factor = 0.5f + 0.5f * car_state.health /
                                         car_state.max_health;  // Como maximo disminuye a la mitad
    // Aceleracion basada en la masa y velocidad actual
    float scaledAcceleration = (1.0f + speedRatio) * phy_const.BASE_ACCELERATION *
                               acceleration_factor * (1.0f + mass_factor) * health_factor;
    if (speed < max_speed) {
        b2Body_ApplyForceToCenter(
                body, {direction.x * scaledAcceleration, direction.y * scaledAcceleration}, true);
    }
}

void CarPhysics::deaccelerate() {
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float rad = car_state.angle * M_PI / 180.0f;
    float forward_speed = velocity.x * cosf(rad) + velocity.y * sinf(rad);

    if (forward_speed > phy_const.MIN_GAME_SPEED) {
        b2Body_SetLinearVelocity(body, {velocity.x * 0.95f, velocity.y * 0.95f});
        car_state.braking = true;
    } else {
        b2Vec2 reverseDirection = {-cosf(rad), -sinf(rad)};
        float maxReverseSpeed =
                (phy_const.BASE_MAX_SPEED * max_speed_factor) * phy_const.REVERSE_SPEED_FACTOR;
        float reverseSpeed =
                std::abs(velocity.x * reverseDirection.x + velocity.y * reverseDirection.y);

        if (reverseSpeed < maxReverseSpeed) {
            b2Body_ApplyForceToCenter(
                    body,
                    {reverseDirection.x * (phy_const.BASE_ACCELERATION * acceleration_factor *
                                           phy_const.REVERSE_ACCELERATION_FACTOR),
                     reverseDirection.y * (phy_const.BASE_ACCELERATION * acceleration_factor *
                                           phy_const.REVERSE_ACCELERATION_FACTOR)},
                    true);
        }
    }
}

void CarPhysics::brake() {
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    if ((std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y)) > 0) {
        car_state.braking = true;
    }

    b2Body_SetLinearVelocity(body, {velocity.x * 0.9f, velocity.y * 0.9f});
}

void CarPhysics::turn_left() {
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed < phy_const.MIN_GAME_SPEED) {
        return;
    }
    car_state.angle -= (phy_const.BASE_ANGLE_ROTATION * drivability_factor);

    if (car_state.angle < 0.0f) {
        car_state.angle += 360.0f;
    }

    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 forward = {cosf(rad), sinf(rad)};
    float forward_speed = velocity.x * forward.x + velocity.y * forward.y;

    b2Vec2 new_velocity = {forward.x * forward_speed, forward.y * forward_speed};
    b2Body_SetLinearVelocity(body, new_velocity);
}

void CarPhysics::turn_right() {

    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed < phy_const.MIN_GAME_SPEED) {
        return;
    }
    car_state.angle += (phy_const.BASE_ANGLE_ROTATION * drivability_factor);

    if (car_state.angle >= 360.f) {
        car_state.angle -= 360.0f;
    }

    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 forward = {cosf(rad), sinf(rad)};
    float forward_speed = velocity.x * forward.x + velocity.y * forward.y;

    b2Vec2 new_velocity = {forward.x * forward_speed, forward.y * forward_speed};
    b2Body_SetLinearVelocity(body, new_velocity);
}

void CarPhysics::handle_nitro() {
    if (nitro_remaining - phy_const.NITRO_USE < 0.0f) {
        return;
    }

    nitro_remaining -= phy_const.NITRO_USE;


    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 direction = {cosf(rad), sinf(rad)};

    b2Body_ApplyForceToCenter(body,
                              {direction.x * (phy_const.BASE_ACCELERATION * acceleration_factor *
                                              phy_const.NITRO_ACCELERATION_FACTOR),
                               direction.y * (phy_const.BASE_ACCELERATION * acceleration_factor *
                                              phy_const.NITRO_ACCELERATION_FACTOR)},
                              true);
}

void CarPhysics::update_position() {
    b2Vec2 pos = b2Body_GetPosition(body);
    car_state.x = pos.x;
    car_state.y = pos.y;
}

void CarPhysics::handle_hits() {
    b2ContactEvents events = b2World_GetContactEvents(world);
    car_state.crashed = false;
    for (int i = 0; i < events.beginCount; i++) {
        if ((events.hitEvents[i].shapeIdA.index1 == shape.index1) ||
            (events.hitEvents[i].shapeIdB.index1 == shape.index1)) {
            handle_hit_event(events.hitEvents[i]);
        }
    }
}

void CarPhysics::handle_hit_event(const b2ContactHitEvent& event) {
    b2Vec2 crash_normal = event.normal;
    b2Vec2 new_normal = crash_normal;
    float other_mass = 0.0f;
    if (event.shapeIdB.index1 == shape.index1) {
        new_normal = {-crash_normal.x, -crash_normal.y};
        other_mass = get_mass_from_shape(event.shapeIdA);
    } else {
        other_mass = get_mass_from_shape(event.shapeIdB);
    }

    float reduced_mass = 0.0f;
    if (other_mass <= 0.0f) {  // Objeto inmovil
        reduced_mass = mass_factor;
    } else {
        reduced_mass = (mass_factor * other_mass) / (mass_factor + other_mass);
    }
    float impact_force = 0.5f * reduced_mass * event.approachSpeed * event.approachSpeed;

    handle_crash(new_normal, impact_force);
}

void CarPhysics::handle_crash_with_bridge() {
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);

    b2Vec2 impact_velocity = {-velocity.x, -velocity.y};
    b2Body_SetLinearVelocity(body, impact_velocity);
}

void CarPhysics::handle_crash(const b2Vec2& normal, const float impact_force) {
    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 forward = {cosf(rad), sinf(rad)};

    float crash_direction = (forward.x * normal.x) + (forward.y * normal.y);
    handle_crash_damage(impact_force, crash_direction);
}

void CarPhysics::handle_crash_damage(const float impact_force, const float crash_direction) {
    float heavy_crash_limit = mass_factor * phy_const.HEAVY_CRASH_LIMIT;
    float medium_crash_limit = mass_factor * phy_const.MEDIUM_CRASH_LIMIT;

    if (crash_direction > 0.7f) {  // golpe frontal
        brake();
        brake();
        if (impact_force > heavy_crash_limit) {
            apply_damage(phy_const.HEAVY_CRASH_DAMAGE);
        } else if (impact_force > medium_crash_limit) {
            apply_damage(phy_const.MEDIUM_CRASH_DAMAGE);
        } else {
            apply_damage(phy_const.LIGHT_CRASH_DAMAGE);
        }
    } else if (crash_direction < -0.7f) {  // golpe de atras, leve
        apply_damage(phy_const.LIGHT_CRASH_DAMAGE);
    } else {  // golpe lateral
        brake();
        if (impact_force > heavy_crash_limit) {
            apply_damage(phy_const.MEDIUM_CRASH_DAMAGE);
        } else {
            apply_damage(phy_const.LIGHT_CRASH_DAMAGE);
        }
    }
}

void CarPhysics::apply_damage(const int dmg) {
    if (car_state.health <= dmg) {
        car_state.health = 0;
        car_state.crashed = true;
        car_state.exploded = true;
    } else {
        if (dmg >= phy_const.MEDIUM_CRASH_DAMAGE) {
            car_state.crashed = true;
        }
        car_state.health -= dmg;
    }
}

float CarPhysics::get_mass_from_shape(b2ShapeId shapeId) {
    if (B2_IS_NULL(shapeId))
        return 0.0f;

    b2BodyId bodyId = b2Shape_GetBody(shapeId);
    if (B2_IS_NULL(bodyId))
        return 0.0f;

    if (b2Body_GetType(bodyId) != b2_dynamicBody)
        return 0.0f;

    const CarPhysics* other = static_cast<CarPhysics*>(b2Body_GetUserData(bodyId));

    if (!other)
        return 0.0f;

    return other->mass_factor;
}

void CarPhysics::set_stats(const float& max_speed, const float& acceleration, const float& mass,
                           const float& drivability) {
    max_speed_factor = max_speed / 100.0f;
    acceleration_factor = acceleration / 100.0f;
    mass_factor = mass / 100.0f;
    drivability_factor = drivability / 100.0f;
    b2Body_SetLinearDamping(body, phy_const.BASE_FRICTION * mass_factor);
    b2Body_SetAngularDamping(body, phy_const.BASE_FRICTION * mass_factor);
}
