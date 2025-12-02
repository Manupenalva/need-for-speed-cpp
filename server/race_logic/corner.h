#ifndef CORNER_H
#define CORNER_H

#include <vector>

#include "../../common/carState.h"

struct Corner {
    int id;
    Position position;
    std::vector<int> neighbors;
};

#endif
