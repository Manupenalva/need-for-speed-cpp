#include "carPhysics.h"

#include <cmath>
#include <iostream>

#include "../common/carState.h"

#define MAX_SPEED 20.0f
#define ACCELERATION 1.0f
#define ANGLE_ROTATION 4

CarPhysics::CarPhysics(b2WorldId world, CarState& car_state, float x, float y):
        world(world), is_colliding(false), car_state(car_state) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = {x, y};
    body = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true;
    b2Polygon box = b2MakeBox(0.1f, 0.2f);
    b2CreatePolygonShape(body, &shapeDef, &box);
}

void CarPhysics::accelerate() {
    car_state.speed += ACCELERATION;

    if (car_state.speed > MAX_SPEED) {
        car_state.speed = MAX_SPEED;
    }
}

void CarPhysics::brake() {
    car_state.speed -= ACCELERATION;

    if (car_state.speed < 0) {
        car_state.speed = 0;
    }
}

void CarPhysics::turn_left() {
    car_state.angle -= ANGLE_ROTATION;

    if (car_state.angle < 0.0f) {
        car_state.angle += 360.0f;
    }
}

void CarPhysics::turn_right() {
    car_state.angle += ANGLE_ROTATION;

    if (car_state.angle >= 360.f) {
        car_state.angle -= 360.0f;
    }
}

void CarPhysics::update_position() {
    float rad = car_state.angle * M_PI / 180.0f;

    car_state.x += cosf(rad) * car_state.speed;
    car_state.y += sinf(rad) * car_state.speed;

    if (car_state.x <= 0) {
        car_state.x = 0;
    }
    if (car_state.y <= 0) {
        car_state.y = 0;
    }

    b2Rot rotation = b2MakeRot(rad);

    b2Body_SetTransform(body, {car_state.x, car_state.y}, rotation);
}
