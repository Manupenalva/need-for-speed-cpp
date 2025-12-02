#include "health_drawer.h"

#include <iostream>

HealthDrawer::HealthDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void HealthDrawer::draw(RenderedState& rendered_state) {
    CarState client_car = rendered_state.client_car;

    int win_h;
    SDL_GetRendererOutputSize(renderer.Get(), nullptr, &win_h);

    Scale scale_factors = get_window_scale_factor();
    float scale_x = scale_factors.x;
    float scale_y = scale_factors.y;

    Bar_dimensions dims = calcular_bar_dimensions(scale_x, scale_y, win_h);

    Bar_colors colors = calcular_bar_colors(dims.w, client_car);

    draw_bar(dims, colors, scale_x, scale_y);
}

Bar_dimensions HealthDrawer::calcular_bar_dimensions(float scale_x, float scale_y, int win_h) {
    Bar_dimensions dims;
    dims.w = static_cast<int>(BAR_W * scale_x);
    dims.h = static_cast<int>(BAR_H * scale_y);
    dims.x = static_cast<int>(BAR_X * scale_x);
    int bar_y_offset = static_cast<int>(BAR_Y_OFFSET * scale_y);
    dims.y = win_h - bar_y_offset - dims.h;
    return dims;
}

Bar_colors HealthDrawer::calcular_bar_colors(int total_w, const CarState& client_car) {
    Bar_colors colors;
    float health_percentage =
            static_cast<float>(client_car.health) / static_cast<float>(client_car.max_health);
    if (health_percentage < 0.0f) {
        health_percentage = 0.0f;
    } else if (health_percentage > 1.0f) {
        health_percentage = 1.0f;
    }

    colors.green_w = static_cast<int>(total_w * health_percentage);
    colors.red_w = total_w - colors.green_w;
    return colors;
}

void HealthDrawer::draw_bar(const Bar_dimensions& dims, const Bar_colors& colors, float scale_x,
                            float scale_y) {
    renderer.SetDrawColor(0, 0, 0, 255);
    renderer.FillRect(SDL2pp::Rect(dims.x - 2 * scale_x, dims.y - 2 * scale_y, dims.w + 4 * scale_x,
                                   dims.h + 4 * scale_y));

    renderer.SetDrawColor(0, 255, 0, 255);
    renderer.FillRect(SDL2pp::Rect(dims.x, dims.y, colors.green_w, dims.h));

    renderer.SetDrawColor(255, 0, 0, 255);
    renderer.FillRect(SDL2pp::Rect(dims.x + colors.green_w, dims.y, colors.red_w, dims.h));
}
