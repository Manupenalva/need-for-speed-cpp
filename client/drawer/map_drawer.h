#ifndef MAP_DRAWER_H
#define MAP_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../config/config_reader.h"
#include "../config/constants.h"
#include "../graphics/sprite.h"
#include "../graphics/texture_manager.h"

class MapDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    sprite get_sprite(int map_id, float focus_x, float focus_y);

    void draw(sprite& map_sprite);
};

#endif
