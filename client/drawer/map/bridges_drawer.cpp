#include "bridges_drawer.h"

BridgesDrawer::BridgesDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void BridgesDrawer::draw(RenderedState& rendered_state) {
    std::vector<Sprite> bridge_sprites =
            texture_manager.get_bridge_sprites(rendered_state.map_type);

    Scale scale_factors = get_window_scale_factor();

    for (const auto& bridge_sprite: bridge_sprites) {
        int wordl_x = bridge_sprite.src_rect.x;
        int world_y = bridge_sprite.src_rect.y;
        int wordl_h = bridge_sprite.src_rect.h;
        int world_w = bridge_sprite.src_rect.w;

        int screen_x = (wordl_x - rendered_state.map_sprite.src_rect.x) * scale_factors.x;
        int screen_y = (world_y - rendered_state.map_sprite.src_rect.y) * scale_factors.y;
        int screen_w = world_w * scale_factors.x;
        int screen_h = wordl_h * scale_factors.y;

        if (screen_x + screen_w < 0 ||
            screen_x > rendered_state.map_sprite.src_rect.w * scale_factors.x ||
            screen_y + screen_h < 0 ||
            screen_y > rendered_state.map_sprite.src_rect.h * scale_factors.y) {
            continue;  // El puente está fuera de la pantalla
        }

        SDL2pp::Rect dst_rect(static_cast<int>(screen_x), static_cast<int>(screen_y),
                              static_cast<int>(screen_w), static_cast<int>(screen_h));

        renderer.Copy(bridge_sprite.texture, bridge_sprite.src_rect, dst_rect);
    }
}
