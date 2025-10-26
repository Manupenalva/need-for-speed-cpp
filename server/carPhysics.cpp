#include "carPhysics.h"

#include <cmath>
#include <iostream>

#include "../common/carState.h"

#define MAX_SPEED 5.0f
#define ACCELERATION 0.05f
#define ANGLE_ROTATION 4

CarPhysics::CarPhysics(b2WorldId world, CarState& car_state, float x, float y):
        world(world), is_colliding(false), car_state(car_state) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {x, y};
    body = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Polygon box = b2MakeBox(2.0f, 4.0f);
    shape = b2CreatePolygonShape(body, &shapeDef, &box);
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
    b2Body_SetLinearVelocity(body, {car_state.x, car_state.y});

    b2ContactEvents events = b2World_GetContactEvents(world);
    for (int i = 0; i < events.beginCount; i++) {
        std::cout << "Hubo un choque" << std::endl;
        if ((events.hitEvents[i].shapeIdA.index1 == shape.index1) ||
            (events.beginEvents[i].shapeIdB.index1 == shape.index1)) {
            handle_hit_event(events.hitEvents[i]);
        }
    }
}

void CarPhysics::handle_hit_event(const b2ContactHitEvent& event) {
    b2Vec2 crash_normal = event.normal;
    b2Vec2 new_normal = crash_normal;

    if (event.shapeIdB.index1 == shape.index1) {
        new_normal = {-crash_normal.x, -crash_normal.y};
    }

    handle_crash(new_normal);
}

void CarPhysics::handle_crash(const b2Vec2& normal) {
    std::cout << "Acabo de chocar con alguien" << std::endl;
    float rad = car_state.angle * M_PI / 180.0f;

    b2Vec2 forward = {cosf(rad), sinf(rad)};

    float crash_direction = (forward.x * normal.x) + (forward.y * normal.y);
    if (crash_direction > 1) {
        accelerate();
    }
    if (crash_direction < 1) {
        car_state.speed = 0;
    } else {
        std::cout << "Chocamos de costado" << std::endl;
    }

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
    b2Body_SetLinearVelocity(body, {car_state.x, car_state.y});
}
