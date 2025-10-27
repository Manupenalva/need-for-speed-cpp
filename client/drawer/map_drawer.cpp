#include "map_drawer.h"


MapDrawer::MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        renderer(renderer), texture_manager(texture_manager) {}


void MapDrawer::draw(int map_id, int section_x, int section_y) {
    // Lógica para dibujar el mapa
    sprite map_sprite = texture_manager.get_map_sprite(map_id, section_x, section_y);

    ConfigReader& config = ConfigReader::get_instance();
    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y, config.get_window_width(),
                          config.get_window_height());

    renderer.Copy(map_sprite.texture, map_sprite.src_rect,
                  dst_rect);  // Dibuja en toda la pantalla
}
