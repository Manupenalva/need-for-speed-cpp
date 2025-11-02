#include "arrow_drawer.h"

ArrowDrawer::ArrowDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void ArrowDrawer::draw(const RenderedState& rendered_state) {
    CarState client_car = rendered_state.client_car;
    float arrow_direction = client_car.checkpoint_arrow.angle;
    sprite arrow_sprite = texture_manager.get_race_sprite(RACE_ARROW, arrow_direction);
    SDL2pp::Rect dst_rect(static_cast<int>(client_car.checkpoint_arrow.x),
                          static_cast<int>(client_car.checkpoint_arrow.y), arrow_sprite.src_rect.w,
                          arrow_sprite.src_rect.h);

    renderer.Copy(arrow_sprite.texture, arrow_sprite.src_rect, dst_rect);
}
