#ifndef CAR_H
#define CAR_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <box2d/box2d.h>

#include "../common/carState.h"


class Car {
private:
    CarInputState input_state;
    CarState state;
    Position next_checkpoint;
    std::vector<float> race_times;
    std::unique_ptr<CarPhysics> physics;

public:
    explicit Car(const uint16_t& id);
    void add_to_world(b2WorldId world, Position start_position, Position first_checkpoint);
    void update_input(const uint8_t& action);
    void update_physics();
    void update_position();
    void handle_hits();
    CarState get_state() const;

private:
    Car(const Car& other) = delete;
    Car& operator=(const Car& other) = delete;
};

#endif
