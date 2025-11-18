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

    if (is_player) {
        draw_border(car, screen_x, screen_y);
    }

    if (car.under_bridge) {
        car_sprite.texture.SetAlphaMod(UNDER_BRIDGE_OPACITY);  // Hacer el coche semitransparente
    }

    SDL2pp::Rect dst_rect(static_cast<int>(screen_x), static_cast<int>(screen_y),
                          car_sprite.src_rect.w, car_sprite.src_rect.h);
    renderer.Copy(car_sprite.texture, car_sprite.src_rect, dst_rect);

    if (car.under_bridge) {
        car_sprite.texture.SetAlphaMod(NORMAL_OPACITY);  // Restaurar opacidad completa
    }
}

void CarDrawer::draw_npcs(const RenderedState& rendered_state) {
    is_player = false;
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
    is_player = true;
    for (const auto& car: rendered_state.state.cars) {
        is_client_car = (car.id == rendered_state.client_car.id);

        CarState predicted_car = calculate_position(car, rendered_state.it_ahead);

        // Ajusta en base al mapa
        float screen_x = predicted_car.x - rendered_state.map_sprite.src_rect.x;
        float screen_y = predicted_car.y - rendered_state.map_sprite.src_rect.y;
        draw_car(predicted_car, screen_x, screen_y);
    }
}

void CarDrawer::draw_border(const CarState& player_car, int screen_x, int screen_y) {
    int padding = 3;
    SDL2pp::Rect border_rect(static_cast<int>(screen_x) - padding,
                             static_cast<int>(screen_y) - padding,
                             player_car.car_type + 2 * padding, player_car.car_type + 2 * padding);
    SDL2pp::Color border_color;
    if (is_client_car) {
        border_color = SDL2pp::Color(0, 255, 0, 255);  // Verde para el coche del cliente
    } else {
        border_color = SDL2pp::Color(255, 0, 0, 255);  // Rojo para otros coches
    }
    renderer.SetDrawColor(border_color);
    renderer.DrawRect(border_rect);
}
