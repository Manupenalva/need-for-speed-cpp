#ifndef BRIDGES_DRAWER_H
#define BRIDGES_DRAWER_H

#include <cmath>
#include <vector>

#include "../../config/constants.h"
#include "../drawer.h"

class BridgesDrawer: public Drawer {
private:
    Rect_dimensions get_bridge_screen_dimensions(const Sprite& bridge_sprite,
                                                 const SDL2pp::Rect& map_rect,
                                                 const Scale& scale_factors);

public:
    explicit BridgesDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
