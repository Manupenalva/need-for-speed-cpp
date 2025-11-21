#include "map_drawer.h"

MapDrawer::MapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void MapDrawer::draw(RenderedState& rendered_state) {
    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);
    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y, w, h);

    renderer.Copy(rendered_state.map_sprite.texture, rendered_state.map_sprite.src_rect,
                  dst_rect);  // Dibuja en toda la pantalla
}
