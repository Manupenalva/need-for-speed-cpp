#ifndef RENDERED_STATE_H
#define RENDERED_STATE_H

#include "../../common/state.h"
#include "../graphics/sprite.h"

struct RenderedState {
    int it_ahead;
    sprite map_sprite;
    State state;
};

#endif
