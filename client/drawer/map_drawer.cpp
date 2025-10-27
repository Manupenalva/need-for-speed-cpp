#include "map_drawer.h"


MapDrawer::MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        renderer(renderer), texture_manager(texture_manager) {}


sprite MapDrawer::get_sprite(int map_id, float focus_x, float focus_y) {
    sprite map_sprite = texture_manager.get_map_sprite(map_id, focus_x, focus_y);

    return map_sprite;
}

void MapDrawer::draw(sprite& map_sprite) {
    ConfigReader& config = ConfigReader::get_instance();
    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y, config.get_window_width(),
                          config.get_window_height());

    renderer.Copy(map_sprite.texture, map_sprite.src_rect,
                  dst_rect);  // Dibuja en toda la pantalla
}
