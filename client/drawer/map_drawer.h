#ifndef MAP_DRAWER_H
#define MAP_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../graphics/texture_manager.h"

class MapDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(int map_id, int section_x, int section_y);
};

#endif
