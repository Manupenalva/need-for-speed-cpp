#ifndef STARTLINE_DRAWER_H
#define STARTLINE_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "drawer.h"

class StartLineDrawer: public Drawer {
public:
    explicit StartLineDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
