#include "car.h"

#include <iostream>

#include "../common/constants.h"

Car::Car(const uint16_t& id, const std::string& name, const float& max_speed,
         const float& acceleration, const float& health, const float& mass,
         const float& drivability, const float& car_long, const float& car_width,
         const int& car_type):
        input_state(),
        state({id, 0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false, false,
               static_cast<uint16_t>(car_type), static_cast<uint16_t>(health)}),
        bridge_layer(BridgeLayer::NONE),
        car_name(name),
        max_speed(max_speed),
        acceleration(acceleration),
        mass(mass),
        drivability(drivability),
        car_long(car_long),
        car_width(car_width),
        max_health(health),
        current_penalization(0.0f) {}

void Car::add_to_world(b2WorldId world, Position start_position) {
    state.x = start_position.x;
    state.y = start_position.y;
    state.angle = 0.0f;
    state.health = static_cast<uint16_t>(max_health);

    physics = std::make_unique<CarPhysics>(world, state, max_speed, acceleration, mass, drivability,
                                           car_long, car_width);
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
        std::cout << "Mejoré la aceleración" << std::endl;
    } else if (action == ACT_IMPROVE_HEALTH) {
        max_health += 5;
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

// lo dejo comentado por ahora por como se maneja el choque
void Car::interact_with_bridge(b2ShapeId /*sensor_shape*/, BridgeLayer sensor_layer) {
    if (bridge_layer == BridgeLayer::NONE) {
        bridge_layer = sensor_layer;
    } else if (bridge_layer == sensor_layer) {
        bridge_layer = BridgeLayer::NONE;
    } else {
        physics->handle_crash_with_bridge();
    }
}

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

void Car::finish_race(float race_time, int position) {
    race_times.push_back(race_time + current_penalization);
    positions.push_back(position);
    current_penalization = 0.0f;
    reset_inputs();
}

void Car::reset_inputs() {
    input_state.accelerating = false;
    input_state.braking = false;
    input_state.turning_left = false;
    input_state.turning_right = false;
}

PlayerState Car::get_player_state() const {
    return PlayerState{state.id,
                       true,
                       static_cast<uint8_t>(get_last_position()),
                       static_cast<uint32_t>(get_result_time() * 1000),
                       static_cast<uint32_t>(get_current_penalization() * 1000),
                       get_properties()};
}

int Car::get_last_position() const {
    if (positions.empty()) {
        return 0;
    }
    return positions.back();
}

float Car::get_result_time() const {
    if (race_times.empty()) {
        return 0.0f;
    }
    return race_times.back();
}

float Car::get_current_penalization() const { return current_penalization; }

CarProperties Car::get_properties() const {
    return CarProperties{static_cast<uint8_t>(state.car_type), static_cast<uint16_t>(max_speed),
                         static_cast<uint16_t>(acceleration),  static_cast<uint16_t>(max_health),
                         static_cast<uint16_t>(mass),          static_cast<uint16_t>(drivability)};
}
