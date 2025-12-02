#ifndef NPC_H
#define NPC_H

#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <box2d/box2d.h>

#include "../../common/carState.h"

#include "carPhysics.h"
#include "corner.h"


class Npc {
private:
    NpcState state;
    b2BodyId body;
    b2ShapeId shape;
    b2WorldId world;
    std::vector<Corner>& corners;
    int current_corner_id;
    int previous_corner_id;

public:
    Npc(const Position& position, std::vector<Corner>& corners, b2WorldId world,
        const float& npc_long, const float& npc_width, const int& car_type);
    void update_position();
    NpcState get_state() const;

private:
    int get_random_number(const int& max_number);
    static float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction,
                                 void* context);
    void choose_first_corner();
    void choose_next_corner();
    float correct_angle(const float& rad_angle);

    Npc(const Npc& other) = delete;
    Npc& operator=(const Npc& other) = delete;
};

#endif
