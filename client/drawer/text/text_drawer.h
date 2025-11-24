#ifndef TEXT_DRAWER_H
#define TEXT_DRAWER_H

#include <algorithm>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "font_manager/font_manager.h"

#define MIN_FONT_SIZE 15
#define MINUTES 60

class TextDrawer {
private:
    SDL2pp::Renderer& renderer;
    FontManager font_manager;

public:
    explicit TextDrawer(SDL2pp::Renderer& renderer);

    void draw_text(const std::string& text, int x, int y, int size, const std::string& font_path);
    void draw_centered_text(const std::string& text, int col_index, Rect_dimensions rect, int y,
                            int font_size, int amount_cols, float scale,
                            const std::string& font_path);
    int get_text_width(const std::string& text, int size);
    int scaled_font(int base_size, float scale);
    std::string format_time(float time);
};

#endif
