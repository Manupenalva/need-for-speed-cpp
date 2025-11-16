#include "arrow_drawer.h"

ArrowDrawer::ArrowDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void ArrowDrawer::draw(const RenderedState& rendered_state) {
    CarState client_car = rendered_state.client_car;
    float arrow_direction = client_car.checkpoint_arrow.angle;
    Sprite_rotation arrow_sprite = texture_manager.get_race_sprite(RACE_ARROW, arrow_direction);

    // Convertir las coordenadas del checkpoint arrow a coordenadas de pantalla
    SDL2pp::Rect map_rect = rendered_state.map_sprite.src_rect;
    int screen_x = static_cast<int>(client_car.checkpoint_arrow.x) - map_rect.x;
    int screen_y = static_cast<int>(client_car.checkpoint_arrow.y) - map_rect.y;

    SDL2pp::Rect dst_rect(screen_x, screen_y, arrow_sprite.sprite.src_rect.w,
                          arrow_sprite.sprite.src_rect.h);

    renderer.Copy(arrow_sprite.sprite.texture, arrow_sprite.sprite.src_rect, dst_rect,
                  arrow_sprite.rotation);
}
