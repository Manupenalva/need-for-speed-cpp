#include "checkpoint_drawer.h"

CheckpointDrawer::CheckpointDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void CheckpointDrawer::draw(const RenderedState& rendered_state) {
    CarState client_car = rendered_state.client_car;
    sprite checkpoint_sprite =
            texture_manager.get_race_sprite(RACE_CHECKPOINT, client_car.checkpoint.angle);
    SDL2pp::Rect dst_rect(static_cast<int>(client_car.checkpoint.x),
                          static_cast<int>(client_car.checkpoint.y), checkpoint_sprite.src_rect.w,
                          checkpoint_sprite.src_rect.h);
    renderer.Copy(checkpoint_sprite.texture, checkpoint_sprite.src_rect, dst_rect);
}
