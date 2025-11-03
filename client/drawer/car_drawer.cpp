#include "car_drawer.h"

CarDrawer::CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void CarDrawer::draw(const RenderedState& rendered_state) {
    SDL2pp::Rect map_rect = rendered_state.map_sprite.src_rect;
    const State& state = rendered_state.state;

    for (const auto& car: state.cars) {
        CarState predicted_car = calculate_position(car, rendered_state.it_ahead);

        // Ajusta en base al mapa
        float screen_x = predicted_car.x - map_rect.x;
        float screen_y = predicted_car.y - map_rect.y;
        draw_car(predicted_car, screen_x, screen_y);
    }
}

CarState CarDrawer::calculate_position(const CarState& car, int iterations_ahead) {
    CarState predicted_car = car;

    // Calcular posición futura basada en la velocidad y las iteraciones adelantadas
    float rad = car.angle * M_PI / HALF(FULL_ROTATION_DEGREES);
    float dx = car.speed * iterations_ahead * cos(rad);
    float dy = car.speed * iterations_ahead * sin(rad);

    predicted_car.x += dx;
    predicted_car.y += dy;

    return predicted_car;
}

void CarDrawer::draw_car(const CarState& car, float screen_x, float screen_y) {
    // Lógica para dibujar el coche
    Sprite car_sprite = texture_manager.get_car_sprite(car.car_type, car.angle);
    SDL2pp::Rect dst_rect(static_cast<int>(screen_x), static_cast<int>(screen_y),
                          car_sprite.src_rect.w, car_sprite.src_rect.h);
    renderer.Copy(car_sprite.texture, car_sprite.src_rect, dst_rect);
}
