#include "map_drawer.h"

MapDrawer::MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void MapDrawer::draw(RenderedState& rendered_state) {
    Scale scale_factors = get_window_scale_factor();
    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y,
                          rendered_state.map_sprite.src_rect.w * scale_factors.x,
                          rendered_state.map_sprite.src_rect.h * scale_factors.y);

    renderer.Copy(rendered_state.map_sprite.texture, rendered_state.map_sprite.src_rect,
                  dst_rect);  // Dibuja en toda la pantalla
}
