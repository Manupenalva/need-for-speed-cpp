#ifndef BRIDGE_H
#define BRIDGE_H

#include "../libs/box2d/include/box2d/box2d.h"

enum BridgeLayer { NONE, TOP, BOTTOM };

struct Bridge {
    b2ShapeId sensor1_up;
    b2ShapeId sensor2_up;
    b2ShapeId sensor1_down;
    b2ShapeId sensor2_down;
    b2ShapeId bridge_area;
};

#endif
