#include "minimap_drawer.h"

MinimapDrawer::MinimapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}


void MinimapDrawer::draw(const RenderedState& rendered_state) {
    SDL2pp::Texture& tex = texture_manager.get_minimap_texture();
    MapResources map_info = calculate_map_rects(rendered_state, tex);
    draw_map(map_info, tex);
    draw_car(map_info, rendered_state.client_car);
    draw_border(map_info);
}

MapResources MinimapDrawer::calculate_map_rects(const RenderedState& rendered_state,
                                                const SDL2pp::Texture& tex) {
    ConfigReader& config = ConfigReader::get_instance();

    int half_w = HALF(tex.GetWidth());
    int half_h = HALF(tex.GetHeight());

    // Centrar la cámara en el coche
    int x = rendered_state.client_car.x - HALF(half_w);
    int y = rendered_state.client_car.y - HALF(half_h);

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x + half_w > tex.GetWidth())
        x = tex.GetWidth() - half_w;
    if (y + half_h > tex.GetHeight())
        y = tex.GetHeight() - half_h;

    SDL2pp::Rect src_rect(x, y, half_w, half_h);

    const int map_wide = config.get_map_wide();
    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y, map_wide, map_wide);  // Minimapa cuadrado

    return {src_rect, dst_rect};
}

void MinimapDrawer::draw_map(const MapResources& map_info, SDL2pp::Texture& texture) {
    renderer.Copy(texture, map_info.src_rect, map_info.dst_rect);
}

void MinimapDrawer::draw_car(const MapResources& map_info, const CarState& client_car) {
    // Calcular posición del coche en el minimapa
    float scale_x = static_cast<float>(map_info.dst_rect.w) / map_info.src_rect.w;
    float scale_y = static_cast<float>(map_info.dst_rect.h) / map_info.src_rect.h;

    int car_minimap_x = static_cast<int>((client_car.x - map_info.src_rect.x) * scale_x);
    int car_minimap_y = static_cast<int>((client_car.y - map_info.src_rect.y) * scale_y);
    Sprite car = texture_manager.get_car_sprite(client_car.car_type, client_car.angle);

    SDL2pp::Rect car_dst_rect(car_minimap_x - car.src_rect.w / RESOURCE_DIVISOR,
                              car_minimap_y - car.src_rect.h / RESOURCE_DIVISOR,
                              HALF(car.src_rect.w), HALF(car.src_rect.h));

    renderer.Copy(car.texture, car.src_rect, car_dst_rect);
}

void MinimapDrawer::draw_border(const MapResources& map_info) {
    SDL2pp::Color border_color(MAP_BORDER_COLOR_R, MAP_BORDER_COLOR_G, MAP_BORDER_COLOR_B,
                               MAP_BORDER_COLOR_A);  // negro
    renderer.SetDrawColor(border_color);
    for (int i = 0; i < BORDER_THICKNESS; ++i) {
        SDL2pp::Rect border_rect(map_info.dst_rect.x - i, map_info.dst_rect.y - i,
                                 map_info.dst_rect.w + 2 * i, map_info.dst_rect.h + 2 * i);
        renderer.DrawRect(border_rect);
    }
}
