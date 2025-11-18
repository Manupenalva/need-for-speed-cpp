#ifndef CHECKPOINT_DRAWER_H
#define CHECKPOINT_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../drawer.h"

class CheckpointDrawer: public Drawer {
public:
    CheckpointDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
