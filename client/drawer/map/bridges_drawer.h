#ifndef BRIDGES_DRAWER_H
#define BRIDGES_DRAWER_H

#include <vector>

#include "../drawer.h"

class BridgesDrawer: public Drawer {
public:
    explicit BridgesDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
