#include "map_drawer.h"

MapDrawer::MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void MapDrawer::draw(const RenderedState& rendered_state) {
    ConfigReader& config = ConfigReader::get_instance();
    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y, config.get_window_width(),
                          config.get_window_height());

    renderer.Copy(rendered_state.map_sprite.texture, rendered_state.map_sprite.src_rect,
                  dst_rect);  // Dibuja en toda la pantalla
}
