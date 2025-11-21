#include "npc.h"

#include <ctime>
#include <iostream>

#define SPEED 50.0f
#define MIN_DISTANCE_TO_CORNER 5.0f

struct RaycastContext {
    bool hit;
    b2ShapeId selfShape;
};

Npc::Npc(const Position& position, std::vector<Corner>& corners, b2WorldId world,
         const float& car_long, const float& car_width, const int& car_type):
        state(position.x, position.y, 0.0f, static_cast<uint16_t>(car_type), false),
        world(world),
        corners(corners),
        current_corner_id(0),
        previous_corner_id(-1) {

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 2.0f;
    bodyDef.angularDamping = 2.0f;
    bodyDef.position = {state.x, state.y};

    body = b2CreateBody(world, &bodyDef);
    b2Body_EnableContactEvents(body, true);
    b2Body_EnableHitEvents(body, true);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Polygon box = b2MakeBox((car_long / 2.0f), (car_width / 2.0f));
    shape = b2CreatePolygonShape(body, &shapeDef, &box);
    b2Shape_EnableContactEvents(shape, true);
    b2Shape_EnableHitEvents(shape, true);

    std::srand(std::time(nullptr));
    choose_first_corner();
}

int Npc::get_random_number(const int& max_number) { return std::rand() % max_number; }

float Npc::RayCastCallback(b2ShapeId shapeId, b2Vec2 /*point*/, b2Vec2 /*normal*/,
                           float /*fraction*/, void* context) {
    RaycastContext* ctx = static_cast<RaycastContext*>(context);
    if (shapeId.index1 == ctx->selfShape.index1) {
        return 1.0f;
    }
    ctx->hit = true;
    return 0.0f;
}

void Npc::choose_first_corner() {
    b2QueryFilter filter = b2DefaultQueryFilter();

    for (const Corner& corner: corners) {
        float x_distance = static_cast<float>(corner.position.x - state.x);
        float y_distance = static_cast<float>(corner.position.y - state.y);

        b2Vec2 origin = {state.x, state.y};
        b2Vec2 translation = {x_distance, y_distance};

        RaycastContext context = {false, shape};
        b2World_CastRay(world, origin, translation, filter, RayCastCallback, &context);

        if (!context.hit) {
            current_corner_id = corner.id;
            return;
        }
    }
}

void Npc::choose_next_corner() {
    const Corner& current = corners[current_corner_id];

    int neighbors_amount = current.neighbors.size();
    if (neighbors_amount == 0)
        return;

    if (neighbors_amount > 1) {
        int random_id = get_random_number(neighbors_amount);
        while (current.neighbors[random_id] == previous_corner_id) {
            random_id = get_random_number(neighbors_amount);
        }
        previous_corner_id = current_corner_id;
        current_corner_id = current.neighbors[random_id];
    } else {
        previous_corner_id = current_corner_id;
        current_corner_id = current.neighbors[0];
    }
}

float Npc::correct_angle(const float& rad_angle) {
    float angle = rad_angle * (180.0f / M_PI);

    if (angle < 0.0f) {
        angle += 360.0f;
    } else if (angle > 360.0f) {
        angle -= 360.0f;
    }

    if (angle >= 315.0f || angle < 45.0f) {
        return 0.0f;
    } else if (angle >= 45.0f && angle < 135.0f) {
        return 90.0f;
    } else if (angle >= 135.0f && angle < 225.0f) {
        return 180.0f;
    } else if (angle >= 225.0f && angle < 315.0f) {
        return 270.0f;
    }

    return angle;
}

void Npc::update_position() {
    const Corner& target = corners[current_corner_id];

    b2Vec2 position = b2Body_GetPosition(body);
    state.x = position.x;
    state.y = position.y;

    b2Vec2 direction = {target.position.x - position.x, target.position.y - position.y};

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < MIN_DISTANCE_TO_CORNER) {
        b2Body_SetLinearVelocity(body, {0, 0});
        choose_next_corner();
        return;
    }

    direction = {direction.x / distance, direction.y / distance};
    float rad = atan2f(direction.y, direction.x);
    state.angle = correct_angle(rad);

    b2Vec2 velocity = {direction.x * SPEED, direction.y * SPEED};
    b2Body_SetLinearVelocity(body, velocity);
}

NpcState Npc::get_state() const { return state; }
