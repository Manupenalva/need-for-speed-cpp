#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <cstdint>

#include "carProperties.h"
struct PlayerState {
    uint16_t player_id;
    bool ready;
    uint8_t previous_position;
    float result_time;
    float next_penalization_time;
    CarProperties car_properties;
};
#endif
