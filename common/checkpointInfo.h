#ifndef CHECKPOINTINFO_H
#define CHECKPOINTINFO_H
#include <cstdint>

#include "constants.h"

struct CheckpointInfo {
    uint16_t id;
    float x;
    float y;
    float angle;
    float radius;  // Radio de activación del checkpoint
    uint8_t type;
};
#endif
