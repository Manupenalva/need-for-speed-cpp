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

    float offsetx = 400 - client_car.x;  // Centrar en pantalla (asumiendo 800x600)
    float offsety = 300 - client_car.y;  // Centrar en pantalla (asumiendo 800x600)

    for (const auto& car: state.cars) {
        CarState predicted_car = car;

        // Calcular posición futura basada en la velocidad y las iteraciones adelantadas
        float rad = car.angle * M_PI / 180.0f;
        float dx = car.speed * iterations_ahead * cos(rad);
        float dy = car.speed * iterations_ahead * sin(rad);

        predicted_car.x += dx;
        predicted_car.y += dy;

        float screen_x = predicted_car.x + offsetx;  // Centrar en pantalla (asumiendo 800x600)
        float screen_y = predicted_car.y + offsety;  // Centrar en pantalla (asumiendo 800x600)
        car_drawer.draw(predicted_car, screen_x, screen_y);
    }
    // TODO: Falta agregar logica de dibujo de gente
}
