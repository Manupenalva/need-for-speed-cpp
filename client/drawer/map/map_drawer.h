#ifndef MAP_DRAWER_H
#define MAP_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../config/constants.h"
#include "../drawer.h"

class MapDrawer: public Drawer {
public:
    explicit MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
