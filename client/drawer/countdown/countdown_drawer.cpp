#include "countdown_drawer.h"

CountdownDrawer::CountdownDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void CountdownDrawer::draw(const RenderedState& rendered_state) {
    if (rendered_state.state.countdown_time <= 0 ||
        rendered_state.state.countdown_time > MAX_NUMBER) {
        return;
    }


    Sprite countdown_sprite =
            texture_manager.get_countdown_sprite(rendered_state.state.countdown_time - 1);

    ConfigReader& config = ConfigReader::get_instance();

    SDL2pp::Rect dst_rect(HALF(config.get_window_width() - countdown_sprite.src_rect.w * 2),
                          HALF(config.get_window_height() - countdown_sprite.src_rect.h * 2),
                          countdown_sprite.src_rect.w * 2, countdown_sprite.src_rect.h * 2);

    renderer.Copy(countdown_sprite.texture, countdown_sprite.src_rect, dst_rect);
}
