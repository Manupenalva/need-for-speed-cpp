#include "car.h"

#include "../common/constants.h"

Car::Car(const uint16_t& id, b2WorldId world):
        input_state(), state(id), next_checkpoint(), world(world) {
    physics = std::make_unique<CarPhysics>(world, state);
}

void Car::update_input(const uint8_t& action) {
    if (action == ACT_ACCEL_PRESS) {
        input_state.accelerating = true;
    } else if (action == ACT_ACCEL_RELEASE) {
        input_state.accelerating = false;
    } else if (action == ACT_BRAKE_PRESS) {
        input_state.braking = true;
    } else if (action == ACT_BRAKE_RELEASE) {
        input_state.braking = false;
    } else if (action == ACT_LEFT_PRESS) {
        input_state.turning_left = true;
    } else if (action == ACT_LEFT_RELEASE) {
        input_state.turning_left = false;
    } else if (action == ACT_RIGHT_PRESS) {
        input_state.turning_right = true;
    } else if (action == ACT_RIGHT_RELEASE) {
        input_state.turning_right = false;
    }
}

void Car::update_physics() {
    if (input_state.accelerating) {
        physics->accelerate();
    }
    if (input_state.braking) {
        physics->brake();
    }
    if (input_state.turning_left) {
        physics->turn_left();
    }
    if (input_state.turning_right) {
        physics->turn_right();
    }
}

void Car::update_position() { physics->update_position(); }

void Car::handle_hits() { physics->handle_hits(); }

CarState Car::get_state() const { return state; }
