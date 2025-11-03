#include "car.h"

#include <iostream>

#include "../common/constants.h"

Car::Car(const uint16_t& id): input_state(), state(id, 0.0f, 0.0f, 0.0f, 0.0f, 0) {}

void Car::add_to_world(b2WorldId world, Position start_position) {
    state.x = start_position.x;
    state.y = start_position.y;

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

CarInfo Car::get_state_info() const { return state; }

bool Car::reached_checkpoint(Position next_checkpoint, float celd_width, float celd_height) {
    if (state.x > (next_checkpoint.x + celd_width / 2.0f))
        return false;
    if (state.x < (next_checkpoint.x - celd_width / 2.0f))
        return false;
    if (state.y > (next_checkpoint.y + celd_height / 2.0f))
        return false;
    if (state.y < (next_checkpoint.y - celd_height / 2.0f))
        return false;

    std::cout << "alcancé un checkpoint en la posicion: " << state.x << ", " << state.y
              << std::endl;
    return true;
}
