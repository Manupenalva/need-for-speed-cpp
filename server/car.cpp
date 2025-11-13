#include "car.h"

#include <iostream>

#include "../common/constants.h"

Car::Car(const uint16_t& id, const std::string& name, const float& max_speed,
         const float& acceleration, const float& health, const float& mass,
         const float& drivability, const float& car_long, const float& car_width,
         const int& car_type):
        input_state(),
        state({id, 0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false, false, static_cast<uint16_t>(car_type), static_cast<uint16_t>(health)}),
        car_name(name),
        max_speed(max_speed),
        acceleration(acceleration),
        mass(mass),
        drivability(drivability),
        car_long(car_long),
        car_width(car_width),
        current_penalization(0.0f) {}

void Car::add_to_world(b2WorldId world, Position start_position) {
    state.x = start_position.x;
    state.y = start_position.y;
    state.angle = 0.0f;

    physics = std::make_unique<CarPhysics>(world, state, max_speed, acceleration, mass, drivability,
                                           car_long, car_width);
    std::cout << "Me agregaron a la carrera, mi velocidad punta es " << max_speed << std::endl;
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

void Car::upgrade_stats(const uint8_t& action) {
    if (action == ACT_IMPROVE_SPEED) {
        max_speed += 5;
        current_penalization += 2.0f;
        std::cout << "Mejoré la velocidad" << std::endl;
    } else if (action == ACT_IMPROVE_ACCELERATION) {
        acceleration += 5;
        current_penalization += 2.0f;
    } else if (action == ACT_IMPROVE_HEALTH) {
        state.health += 5;
        current_penalization += 2.0f;
    } else if (action == ACT_IMPROVE_MASS) {
        mass += 5;
        current_penalization += 2.0f;
    } else if (action == ACT_IMPROVE_HANDLING) {
        drivability += 5;
        current_penalization += 2.0f;
    }
}

void Car::update_physics() {
    state.braking = false;
    if (input_state.accelerating) {
        physics->accelerate();
    }
    if (input_state.braking) {
        physics->deaccelerate();
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

void Car::finish_race(float race_time) {
    race_times.push_back(race_time + current_penalization);
    current_penalization = 0.0f;
    reset_inputs();
}

void Car::reset_inputs() {
    input_state.accelerating = false;
    input_state.braking = false;
    input_state.turning_left = false;
    input_state.turning_right = false;
}
