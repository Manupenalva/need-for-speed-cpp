#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <cstdint>

#include "carProperties.h"
struct PlayerState {
    uint16_t player_id;
    bool ready;
    uint8_t previous_position;
    uint32_t result_time;             // in milliseconds
    uint32_t next_penalization_time;  // in milliseconds
    CarProperties car_properties;
};
#endif
