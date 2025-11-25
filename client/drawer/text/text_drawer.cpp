#include "text_drawer.h"

TextDrawer::TextDrawer(SDL2pp::Renderer& renderer): renderer(renderer), font_manager() {}

void TextDrawer::draw_text(const std::string& text, int x, int y, int size,
                           const std::string& font_path, SDL2pp::Color color) {
    SDL2pp::Font& font = font_manager.get_font({size, font_path});
    SDL2pp::Surface surface = font.RenderText_Solid(text, color);
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(x, y, surface.GetWidth(), surface.GetHeight()));
}

void TextDrawer::draw_centered_text(const std::string& text, int col_index, Rect_dimensions rect,
                                    int y, int font_size, int amount_cols, float scale,
                                    const std::string& font_path) {
    int col_w = HALF(rect.w / amount_cols);
    int col_x = rect.x + col_index * col_w;

    int size = scaled_font(font_size, scale);
    int text_w = get_text_width(text, size);

    int x = col_x + HALF(col_w - text_w);

    draw_text(text, x, y, size, font_path);
}

int TextDrawer::get_text_width(const std::string& text, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Surface surface = font.RenderText_Solid(text, SDL2pp::Color(WHITE_COLOR(255)));
    return surface.GetWidth();
}

int TextDrawer::scaled_font(int base_size, float scale) {
    return std::max(MIN_FONT_SIZE, static_cast<int>(base_size * scale));
}

std::string TextDrawer::format_time(float time) {
    int minutes = static_cast<int>(time) / MINUTES;
    float seconds = time - (minutes * MINUTES);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%06.3f", minutes, seconds);
    return std::string(buffer);
}
