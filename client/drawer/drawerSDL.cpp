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

    map_drawer.draw(1, client_car.x, client_car.y);

    for (const auto& car: state.cars) {
        CarState predicted_car = car;
        // Calcular posición futura basada en la velocidad y las iteraciones adelantadas
        float dx = car.speed * iterations_ahead * cos(car.angle);
        float dy = car.speed * iterations_ahead * sin(car.angle);

        predicted_car.x += dx;
        predicted_car.y += dy;

        float screen_x =
                predicted_car.x - client_car.x + 400;  // Centrar en pantalla (asumiendo 800x600)
        float screen_y =
                predicted_car.y - client_car.y + 300;  // Centrar en pantalla (asumiendo 800x600)
        car_drawer.draw(predicted_car, screen_x, screen_y);
    }

    // TODO: Falta agregar logica de dibujo de gente
}
