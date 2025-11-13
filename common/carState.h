#ifndef CARSTATE_H
#define CARSTATE_H

#include <cstdint>
#include <memory>

#include "checkpointArrow.h"
#include "checkpointInfo.h"
#include "npcState.h"

struct CarState {
    uint16_t id = 0;
    float x = 0.0f;
    float y = 0.0f;
    float angle = 0.0f;
    float speed = 0.0f;
    uint16_t lap = 0;
    CheckpointInfo checkpoint{};
    CheckpointArrow checkpoint_arrow{};
    bool crashed = false;
    bool under_bridge = false;
    bool braking = false;
    uint16_t car_type = 0;
    uint16_t health = 100;

    // Constructor por defecto
    CarState() = default;

    // Constructor para facilitar el dibujo de npcs
    explicit CarState(const NpcState& npc):
            x(npc.x), y(npc.y), angle(npc.angle), car_type(npc.car_type) {}

    // Constructor completo
    CarState(uint16_t id, float x, float y, float angle, float speed, uint16_t lap,
             const CheckpointInfo& checkpoint, const CheckpointArrow& checkpoint_arrow,
             bool crashed, uint16_t car_type, uint16_t health):
            id(id),
            x(x),
            y(y),
            angle(angle),
            speed(speed),
            lap(lap),
            checkpoint(checkpoint),
            checkpoint_arrow(checkpoint_arrow),
            crashed(crashed),
            car_type(car_type),
            health(health) {}
};

// esto podría ir dentro de CarState para no repetir.
struct CarInfo {
    uint16_t id;
    float x;
    float y;
    float angle;
    float speed;
    uint16_t lap;
    bool crashed;
    bool under_bridge;
    bool braking;
    uint16_t car_type;
    uint16_t health;

    CarInfo(uint16_t id, float x, float y, float angle, float speed, uint16_t lap,
            bool crashed, bool under_bridge, bool braking, uint16_t car_type, uint16_t health):
            id(id),
            x(x),
            y(y),
            angle(angle),
            speed(speed),
            lap(lap),
            crashed(crashed),
            under_bridge(under_bridge),
            braking(braking),
            car_type(car_type),
            health(health) {}
};

struct CarInputState {
    bool accelerating;
    bool braking;
    bool turning_right;
    bool turning_left;

    CarInputState(bool accelerating, bool braking, bool turning_right, bool turning_left):
            accelerating(accelerating),
            braking(braking),
            turning_right(turning_right),
            turning_left(turning_left) {}

    CarInputState() :
            accelerating(false),
            braking(false),
            turning_right(false),
            turning_left(false) {}
};

struct Position {
    int x;
    int y;
};

#endif
