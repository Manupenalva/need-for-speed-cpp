#ifndef MAP_DRAWER_H
#define MAP_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../graphics/texture_manager.h"

class MapDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            renderer(renderer), texture_manager(texture_manager) {}

    void draw(int map_id, int section_x, int section_y) {
        // Lógica para dibujar el mapa
        sprite map_sprite = texture_manager.get_map_sprite(map_id, section_x, section_y);
        SDL2pp::Rect dst_rect(0, 0, 800, 600);
        renderer.Copy(map_sprite.texture, map_sprite.src_rect,
                      dst_rect);  // Dibuja en toda la pantalla
    }
};

#endif
