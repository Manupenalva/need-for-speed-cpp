#include "checkpoint_drawer.h"

CheckpointDrawer::CheckpointDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void CheckpointDrawer::draw(RenderedState& rendered_state) {
    CarState client_car = rendered_state.client_car;

    Sprite_rotation checkpoint_sprite = texture_manager.get_race_sprite(
            client_car.checkpoint.type, client_car.checkpoint.angle);  // type 1 o 2

    SDL2pp::Rect map_rect = rendered_state.map_sprite.src_rect;
    int screen_x = static_cast<int>(client_car.checkpoint.x) - map_rect.x;
    int screen_y = static_cast<int>(client_car.checkpoint.y) - map_rect.y;

    SDL2pp::Rect dst_rect(screen_x, screen_y, checkpoint_sprite.sprite.src_rect.w,
                          checkpoint_sprite.sprite.src_rect.h);
    renderer.Copy(checkpoint_sprite.sprite.texture, checkpoint_sprite.sprite.src_rect, dst_rect,
                  checkpoint_sprite.rotation);
}
