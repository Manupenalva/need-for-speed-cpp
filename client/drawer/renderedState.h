#ifndef RENDERED_STATE_H
#define RENDERED_STATE_H

#include "../../common/carState.h"
#include "../../common/state.h"
#include "../graphics/sprite.h"

struct RenderedState {
    int it_ahead;
    CarState client_car;
    Sprite map_sprite;
    State state;
    uint8_t map_number;
    MinimapInfo minimap_info;
};

#endif
