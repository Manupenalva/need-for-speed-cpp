#include "carPhysics.h"

#include <cmath>

#define MAX_SPEED 20.0f
#define ACCELERATION 5.0f
#define ANGLE_ROTATION 4

CarPhysics::CarPhysics(b2WorldId world, float x, float y): world(world), is_colliding(false) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {x, y};
    body = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Polygon box = b2MakeBox(0.5f, 1.0f);
    b2CreatePolygonShape(body, &shapeDef, &box);
}

void CarPhysics::accelerate() {
    b2Transform transform = b2Body_GetTransform(body);
    float angle = b2Rot_GetAngle(transform.q);

    b2Vec2 direction = {cosf(angle), sinf(angle)};
    b2Vec2 force = {direction.x * ACCELERATION, direction.y * ACCELERATION};

    b2Body_ApplyForceToCenter(body, force, true);

    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > MAX_SPEED) {
        b2Vec2 normalized_direction = {velocity.x / speed, velocity.y / speed};
        b2Vec2 max_velocity = {normalized_direction.x * MAX_SPEED,
                               normalized_direction.y * MAX_SPEED};
        b2Body_SetLinearVelocity(body, max_velocity);
    }
}

void CarPhysics::brake() {
    // tmb se puede frenar usando la velocidad lineal * 0.9
    b2Transform transform = b2Body_GetTransform(body);
    float angle = b2Rot_GetAngle(transform.q);
    b2Vec2 direction = {cosf(angle), sinf(angle)};
    b2Body_ApplyForceToCenter(body, (-ACCELERATION) * direction, true);
}

void CarPhysics::turn_left() {
    b2Transform transform = b2Body_GetTransform(body);
    float angle = b2Rot_GetAngle(transform.q);
    float rad_rotation = ANGLE_ROTATION * (M_PI / 180.0f);

    b2Vec2 position = b2Body_GetPosition(body);
    b2Rot new_rotation = b2MakeRot(angle - rad_rotation);
    b2Body_SetTransform(body, position, new_rotation);
}

void CarPhysics::turn_right() {
    b2Transform transform = b2Body_GetTransform(body);
    float angle = b2Rot_GetAngle(transform.q);
    float rad_rotation = ANGLE_ROTATION * (M_PI / 180.0f);

    b2Vec2 position = b2Body_GetPosition(body);
    b2Rot new_rotation = b2MakeRot(angle - rad_rotation);
    b2Body_SetTransform(body, position, new_rotation);
}

float CarPhysics::get_x_position() { return b2Body_GetPosition(body).x; }

float CarPhysics::get_y_position() { return b2Body_GetPosition(body).y; }

float CarPhysics::get_angle() {
    b2Transform transform = b2Body_GetTransform(body);
    float rad_angle = b2Rot_GetAngle(transform.q);
    return (rad_angle * (180.0f / M_PI));
}

float CarPhysics::get_speed() {
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    return speed;
}
