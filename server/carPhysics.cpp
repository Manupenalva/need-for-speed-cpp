#include "carPhysics.h"

#include <cmath>
#include <iostream>

#include "../common/carState.h"

#define MAX_SPEED 5000.0f
#define MIN_SPEED 100.0f
#define ACCELERATION 400.0f
#define ANGLE_ROTATION 4

CarPhysics::CarPhysics(b2WorldId world, CarState& car_state, float x, float y):
        world(world), life(100.0f), car_state(car_state) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 2.0f;
    bodyDef.angularDamping = 5.0f;
    bodyDef.position = {x, y};
    body = b2CreateBody(world, &bodyDef);
    b2Body_EnableContactEvents(body, true);
    b2Body_EnableHitEvents(body, true);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Polygon box = b2MakeBox(20.5f, 10.5f);
    shape = b2CreatePolygonShape(body, &shapeDef, &box);
    b2Shape_EnableContactEvents(shape, true);
    b2Shape_EnableHitEvents(shape, true);
}

void CarPhysics::accelerate() {
    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 direction = {cosf(rad), sinf(rad)};

    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    if (speed < MIN_SPEED) {
        speed = MIN_SPEED;
        b2Vec2 initial_velocity = {direction.x * MIN_SPEED, direction.y * MIN_SPEED};
        b2Body_SetLinearVelocity(body, initial_velocity);
    }

    if (speed < MAX_SPEED) {
        b2Body_ApplyForceToCenter(body, {direction.x * ACCELERATION, direction.y * ACCELERATION},
                                  true);
    }
}

void CarPhysics::brake() {
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);

    b2Body_SetLinearVelocity(body, {velocity.x * 0.9f, velocity.y * 0.9f});
}

void CarPhysics::turn_left() {
    car_state.angle -= ANGLE_ROTATION;

    if (car_state.angle < 0.0f) {
        car_state.angle += 360.0f;
    }

    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 direction = {cosf(rad), sinf(rad)};

    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    b2Vec2 new_velocity = {direction.x * speed, direction.y * speed};
    b2Body_SetLinearVelocity(body, new_velocity);
}

void CarPhysics::turn_right() {
    car_state.angle += ANGLE_ROTATION;

    if (car_state.angle >= 360.f) {
        car_state.angle -= 360.0f;
    }

    float rad = car_state.angle * M_PI / 180.0f;
    b2Vec2 direction = {cosf(rad), sinf(rad)};

    b2Vec2 velocity = b2Body_GetLinearVelocity(body);
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    b2Vec2 new_velocity = {direction.x * speed, direction.y * speed};
    b2Body_SetLinearVelocity(body, new_velocity);
}

void CarPhysics::update_position() {
    b2Vec2 pos = b2Body_GetPosition(body);
    car_state.x = pos.x;
    car_state.y = pos.y;
}

void CarPhysics::handle_hits() {
    b2ContactEvents events = b2World_GetContactEvents(world);
    for (int i = 0; i < events.beginCount; i++) {
        std::cout << "Chocaron dos cajas" << std::endl;
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
    if (crash_direction > 0.7f) {  // le doy de frente, disminuye la velocidad y resta vida
        brake();
        brake();
        life -= 30;
    } else if (crash_direction < -0.7f) {  // me pega el, solo resta vida
        life -= 20;
    } else {  // me dan en el costado, pierdo menos vida y velocidad que de frente
        brake();
        life -= 10;
    }
}
