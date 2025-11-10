#include "car_drawer.h"

#include <vector>

CarDrawer::CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void CarDrawer::draw(const RenderedState& rendered_state) {
    draw_clients_cars(rendered_state);
    draw_npcs(rendered_state);
}

CarState CarDrawer::calculate_position(const CarState& car, const int iterations_ahead) {
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

void CarDrawer::draw_npcs(const RenderedState& rendered_state) {
    for (const auto& npc: rendered_state.state.npcs) {
        CarState npc_car_state(npc);  // Convertir NpcState a CarState
        CarState predicted_npc = calculate_position(npc_car_state, rendered_state.it_ahead);

        // Ajusta en base al mapa
        float screen_x = predicted_npc.x - rendered_state.map_sprite.src_rect.x;
        float screen_y = predicted_npc.y - rendered_state.map_sprite.src_rect.y;
        draw_car(predicted_npc, screen_x, screen_y);
    }
}

void CarDrawer::draw_clients_cars(const RenderedState& rendered_state) {
    for (const auto& car: rendered_state.state.cars) {
        CarState predicted_car = calculate_position(car, rendered_state.it_ahead);

        // Ajusta en base al mapa
        float screen_x = predicted_car.x - rendered_state.map_sprite.src_rect.x;
        float screen_y = predicted_car.y - rendered_state.map_sprite.src_rect.y;
        draw_car(predicted_car, screen_x, screen_y);
    }
}
