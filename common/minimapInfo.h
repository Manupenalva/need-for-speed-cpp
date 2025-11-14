#ifndef MINIMAP_INFO_H
#define MINIMAP_INFO_H

#include <cstdint>
#include <vector>
#include "checkpointArrow.h"
#include "checkpointInfo.h"

struct MinimapInfo {
    std::vector<CheckpointInfo> checkpoints;
    std::vector<CheckpointArrow> arrows;
};

#endif