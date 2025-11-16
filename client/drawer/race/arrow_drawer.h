#ifndef ARROW_DRAWER_H
#define ARROW_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../drawer.h"

class ArrowDrawer: public Drawer {
public:
    explicit ArrowDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
