#include "car_drawer.h"

#include <vector>

CarDrawer::CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void CarDrawer::draw(RenderedState& rendered_state) {
    draw_clients_cars(rendered_state);
    draw_npcs(rendered_state);
}

void CarDrawer::draw_npcs(const RenderedState& rendered_state) {
    is_player = false;
    for (const auto& npc: rendered_state.state.npcs) {
        CarState npc_car_state(npc);  // Convertir NpcState a CarState
        CarState predicted_npc = calculate_position(npc_car_state, rendered_state.it_ahead);

        CarScreenPos screen_pos =
                calculate_map_scale(predicted_npc, rendered_state.map_sprite.src_rect);

        draw_car(predicted_npc, screen_pos);
    }
}

void CarDrawer::draw_clients_cars(RenderedState& rendered_state) {
    is_player = true;
    for (const auto& car: rendered_state.state.cars) {
        is_client_car = (car.id == rendered_state.client_car.id);

        CarState predicted_car = calculate_position(car, rendered_state.it_ahead);

        CarScreenPos screen_pos =
                calculate_map_scale(predicted_car, rendered_state.map_sprite.src_rect);

        SDL2pp::Rect dst_rect = draw_car(predicted_car, screen_pos);
        if (is_client_car) {
            rendered_state.client_car_screen_rect = dst_rect;
        }
    }
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

CarScreenPos CarDrawer::calculate_map_scale(const CarState& car, const SDL2pp::Rect& map_rect) {
    Scale scale_factors = get_window_scale_factor();

    // Ajusta en base al mapa
    float screen_x = (car.x - map_rect.x) * scale_factors.x;
    float screen_y = (car.y - map_rect.y) * scale_factors.y;
    return {screen_x, screen_y, scale_factors.x, scale_factors.y};
}

SDL2pp::Rect CarDrawer::draw_car(const CarState& car, const CarScreenPos& screen_pos) {
    // Lógica para dibujar el coche
    Sprite car_sprite = texture_manager.get_car_sprite(car.car_type, car.angle);

    SDL2pp::Rect dst_rect(static_cast<int>(screen_pos.x), static_cast<int>(screen_pos.y),
                          car_sprite.src_rect.w * screen_pos.scale_x,
                          car_sprite.src_rect.h * screen_pos.scale_y);


    if (!is_drawable(dst_rect)) {
        return dst_rect;  // No dibujar si está fuera del área de la pantalla
    }

    appply_visual_effects(car, screen_pos, car_sprite);

    renderer.Copy(car_sprite.texture, car_sprite.src_rect, dst_rect);
    car_sprite.texture.SetAlphaMod(NORMAL_OPACITY);  // Restaurar opacidad completa

    return dst_rect;
}

bool CarDrawer::is_drawable(const SDL2pp::Rect& car_rect) {
    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);
    SDL2pp::Rect screen_rect(0, 0, w, h);

    return SDL_HasIntersection(&car_rect, &screen_rect);
}

void CarDrawer::appply_visual_effects(const CarState& car, const CarScreenPos& screen_pos,
                                      Sprite& car_sprite) {
    if (is_player) {
        draw_border(car, screen_pos.x, screen_pos.y);
    }

    if (car.under_bridge) {
        car_sprite.texture.SetAlphaMod(UNDER_BRIDGE_OPACITY);  // Hacer el coche semitransparente
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
