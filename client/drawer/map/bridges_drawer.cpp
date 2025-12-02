#include "bridges_drawer.h"

BridgesDrawer::BridgesDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void BridgesDrawer::draw(RenderedState& rendered_state) {
    std::vector<Sprite> bridge_sprites =
            texture_manager.get_bridge_sprites(rendered_state.map_type);

    Scale scale_factors = get_window_scale_factor();

    int map_dst_w = static_cast<int>(rendered_state.map_sprite.src_rect.w * scale_factors.x);
    int map_dst_h = static_cast<int>(rendered_state.map_sprite.src_rect.h * scale_factors.y);

    for (const auto& bridge_sprite: bridge_sprites) {
        Rect_dimensions screen_dimensions = get_bridge_screen_dimensions(
                bridge_sprite, rendered_state.map_sprite.src_rect, scale_factors);

        if (screen_dimensions.x + screen_dimensions.w < MAP_MIN_X ||
            screen_dimensions.x > map_dst_w ||
            screen_dimensions.y + screen_dimensions.h < MAP_MIN_Y ||
            screen_dimensions.y > map_dst_h) {
            continue;
        }

        SDL2pp::Rect dst_rect(screen_dimensions.x, screen_dimensions.y, screen_dimensions.w,
                              screen_dimensions.h);

        renderer.Copy(bridge_sprite.texture, bridge_sprite.src_rect, dst_rect);
    }
}

Rect_dimensions BridgesDrawer::get_bridge_screen_dimensions(const Sprite& bridge_sprite,
                                                            const SDL2pp::Rect& map_rect,
                                                            const Scale& scale_factors) {
    int world_x = bridge_sprite.src_rect.x;
    int world_y = bridge_sprite.src_rect.y;
    int world_h = bridge_sprite.src_rect.h;
    int world_w = bridge_sprite.src_rect.w;

    float fx = (static_cast<float>(world_x) - static_cast<float>(map_rect.x)) * scale_factors.x;
    float fy = (static_cast<float>(world_y) - static_cast<float>(map_rect.y)) * scale_factors.y;
    float fw = static_cast<float>(world_w) * scale_factors.x;
    float fh = static_cast<float>(world_h) * scale_factors.y;

    int screen_x = static_cast<int>(std::lround(fx));
    int screen_y = static_cast<int>(std::lround(fy));
    int screen_w = static_cast<int>(std::lround(fw));
    int screen_h = static_cast<int>(std::lround(fh));

    return {screen_x, screen_y, screen_w, screen_h};
}
