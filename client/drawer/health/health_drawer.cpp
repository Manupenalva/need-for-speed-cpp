#include "health_drawer.h"

#include <iostream>

HealthDrawer::HealthDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void HealthDrawer::draw(const RenderedState& rendered_state) {
    CarState client_car = rendered_state.client_car;
    ConfigReader& config = ConfigReader::get_instance();
    int y = config.get_window_height() - BAR_H - BAR_Y_OFFSET;

    float health_percentage =
            static_cast<float>(client_car.health) / static_cast<float>(client_car.total_health);
    if (health_percentage < 0.0f) {
        health_percentage = 0.0f;
    } else if (health_percentage > 1.0f) {
        health_percentage = 1.0f;
    }

    int green_w = static_cast<int>(BAR_W * health_percentage);
    int red_w = BAR_W - green_w;

    renderer.SetDrawColor(0, 0, 0, 255);
    renderer.FillRect(SDL2pp::Rect(BAR_X - 2, y - 2, BAR_W + 4, BAR_H + 4));

    renderer.SetDrawColor(0, 255, 0, 255);
    renderer.FillRect(SDL2pp::Rect(BAR_X, y, green_w, BAR_H));

    renderer.SetDrawColor(255, 0, 0, 255);
    renderer.FillRect(SDL2pp::Rect(BAR_X + green_w, y, red_w, BAR_H));
}
