#ifndef CHECKPOINTINFO_H
#define CHECKPOINTINFO_H
#include <cstdint>

struct CheckpointInfo {
    uint16_t id;
    float x;
    float y;
    float angle;
    float radius;  // Radio de activación del checkpoint
};
#endif
