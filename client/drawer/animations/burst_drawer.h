#ifndef CRASH_DRAWER_H
#define CRASH_DRAWER_H

#include "animations_drawer.h"

#define AMOUNT_OF_BURST_FRAMES 8
#define BURST_DURATION 15

class BurstDrawer: public AnimationsDrawer {
public:
    BurstDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            AnimationsDrawer(
                    renderer, texture_manager,
                    [](const CarState& car_state) { return car_state.exploded; },
                    [](TextureManager& texture_manager, int frame) {
                        return texture_manager.get_burst_sprite(frame);
                    },
                    AMOUNT_OF_BURST_FRAMES, BURST_DURATION) {}
};

#endif
