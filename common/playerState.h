#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <cstdint>

struct PlayerState {
    uint8_t player_id;
    uint8_t previous_position;
    uint32_t result_time;             // in milliseconds
    uint32_t next_penalization_time;  // in milliseconds
    CarProperties car_properties;
};
#endif
