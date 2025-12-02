#ifndef CAR_H
#define CAR_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <box2d/box2d.h>

#include "../../common/carProperties.h"
#include "../../common/carState.h"
#include "../../common/playerState.h"

#include "bridge.h"
#include "carConstants.h"
#include "carPhysics.h"


class Car {
private:
    CarInputState input_state;
    CarInfo state;
    std::vector<float> race_times;
    std::vector<int> positions;
    std::unique_ptr<CarPhysics> physics;

    BridgeLayer bridge_layer;

    std::string car_name;
    float max_speed;
    float acceleration;
    float mass;
    float drivability;
    float car_long;
    float car_width;
    float max_health;

    float current_penalization;
    float total_penalization;

    b2WorldId curr_world;
    std::shared_ptr<CarConstants> car_constants;

    uint32_t brake_counter;

public:
    Car(const uint16_t& id, const std::string& name, const float& max_speed,
        const float& acceleration, const float& health, const float& mass, const float& drivability,
        const float& car_long, const float& car_width, const int& car_type,
        std::shared_ptr<CarConstants> car_constants);

    void add_to_world(b2WorldId world, Position start_position);
    void update_input(const uint8_t& action);
    void upgrade_stats(const uint8_t& action);
    void update_physics();
    void update_position();
    void handle_hits();
    CarInfo get_state_info() const;
    bool reached_checkpoint(Position next_checkpoint, float celd_width, float celd_height);
    void finish_race(float race_time, int position);
    void reset_inputs();
    void interact_with_bridge(BridgeLayer sensor_layer);
    PlayerState get_player_state() const;
    void activate_infinite_health();
    void explode();
    void maximize_stats();
    bool exploded();
    float get_total_time() const;
    float get_penalization_time() const;
    float get_total_penalization() const;

    Car(Car&& other) = default;
    Car& operator=(Car&& other) = default;
    Car():
            input_state(false, false, false, false, false),
            state(0, 0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false, false, 0, 0, 0),
            bridge_layer(BridgeLayer::NONE),
            car_name("default"),
            max_speed(0),
            acceleration(0),
            mass(0),
            drivability(0),
            car_long(0),
            car_width(0),
            max_health(0),
            current_penalization(0),
            total_penalization(0),
            curr_world(),
            brake_counter(0) {}

private:
    Car(const Car& other) = delete;
    Car& operator=(const Car& other) = delete;
    int get_last_position() const;
    float get_result_time() const;
    CarProperties get_properties() const;
};

#endif
