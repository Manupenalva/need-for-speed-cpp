#include "remaining_time_drawer.h"

RemainingTimeDrawer::RemainingTimeDrawer(SDL2pp::Renderer& renderer,
                                         TextureManager& texture_manager):
        Drawer(renderer, texture_manager), text_drawer(renderer) {}

void RemainingTimeDrawer::draw(RenderedState& rendered_state) {
    float remaining_time = rendered_state.state.remaining_time;

    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);

    Scale scale = get_window_scale_factor();
    float scale_factor = std::min(scale.x, scale.y);

    int font_size = text_drawer.scaled_font(TIME_FONT_SIZE, scale_factor);

    std::string time_text = text_drawer.format_time(remaining_time);
    int text_width = text_drawer.get_text_width(time_text, font_size);

    int margin = INITIAL_MARGIN * scale_factor;

    int x = w - text_width - margin;
    int y = margin;

    text_drawer.draw_text(time_text, x + SHADOW_OFFSET * scale_factor,
                          y + SHADOW_OFFSET * scale_factor, font_size, FONT_PATH,
                          SDL2pp::Color(0, 0, 0, 150));

    text_drawer.draw_text(time_text, x, y, font_size, FONT_PATH);
}
