#include "drawerSDL.h"

#include <utility>
#include <vector>

#include <SDL2/SDL.h>

DrawerSDL::DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id):
        client_id(client_id),
        renderer(renderer),
        texture_manager(texture_manager),
        car_drawer(renderer, texture_manager),
        people_drawer(renderer, texture_manager),
        map_drawer(renderer, texture_manager) {}

void DrawerSDL::update_state(const ServerMessageDTO& msg, int iterations_ahead) {
    if (msg.type != MsgType::STATE_UPDATE) {
        return;
    }

    const State& state = msg.state;
    std::vector<CarState> cars = state.cars;

    CarState client_car = cars[client_id];

    SDL2pp::Rect map_rect = draw_map(client_car);
    draw_cars(state, map_rect, iterations_ahead);
    // TODO: Falta agregar logica de dibujo de gente
}

SDL2pp::Rect DrawerSDL::draw_map(const CarState& client_car) {
    sprite map_sprite = map_drawer.get_sprite(ConfigReader::get_instance().get_map_id(),
                                              client_car.x, client_car.y);
    map_drawer.draw(map_sprite);
    return map_sprite.src_rect;
}

void DrawerSDL::draw_cars(const State& state, const SDL2pp::Rect& map_rect, int iterations_ahead) {
    for (const auto& car: state.cars) {
        CarState predicted_car = car;

        // Calcular posición futura basada en la velocidad y las iteraciones adelantadas
        float rad = car.angle * M_PI / HALF(FULL_ROTATION_DEGREES);
        float dx = car.speed * iterations_ahead * cos(rad);
        float dy = car.speed * iterations_ahead * sin(rad);

        predicted_car.x += dx;
        predicted_car.y += dy;

        float screen_x = predicted_car.x - map_rect.x;
        float screen_y = predicted_car.y - map_rect.y;
        car_drawer.draw(predicted_car, screen_x, screen_y);
    }
}
