#ifndef CARPROPERTIES_H
#define CARPROPERTIES_H

#include <cstdint>

struct CarProperties {
    uint8_t car_id;
    uint16_t max_speed;
    uint16_t acceleration;
    uint16_t max_health;
    uint16_t mass;     // masa
    uint16_t control;  // controlabilidad maxima
};
#endif
