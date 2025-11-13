#ifndef CAR_H
#define CAR_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <box2d/box2d.h>

#include "../common/carState.h"

#include "carPhysics.h"


class Car {
private:
    CarInputState input_state;
    CarInfo state;
    std::vector<float> race_times;
    std::unique_ptr<CarPhysics> physics;

    std::string car_name;
    float max_speed;
    float acceleration;
    float mass;
    float drivability;
    float car_long;
    float car_width;

    float current_penalization;

public:
    Car(const uint16_t& id, const std::string& name, const float& max_speed,
        const float& acceleration, const float& health, const float& mass, const float& drivability,
        const float& car_long, const float& car_width, const int& car_type);

    void add_to_world(b2WorldId world, Position start_position);
    void update_input(const uint8_t& action);
    void upgrade_stats(const uint8_t& action);
    void update_physics();
    void update_position();
    void handle_hits();
    CarInfo get_state_info() const;
    bool reached_checkpoint(Position next_checkpoint, float celd_width, float celd_height);
    void finish_race(float race_time);
    void reset_inputs();

    Car(Car&& other) = default;
    Car& operator=(Car&& other) = default;
    Car():
            input_state(false, false, false, false),
            state(0, 0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false, false, 0, 0),
            car_name("default"),
            max_speed(0),
            acceleration(0),
            mass(0),
            drivability(0),
            car_long(0),
            car_width(0),
            current_penalization(0) {}

private:
    Car(const Car& other) = delete;
    Car& operator=(const Car& other) = delete;
};

#endif
