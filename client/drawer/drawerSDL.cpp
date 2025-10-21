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

DrawerSDL::~DrawerSDL() = default;

void DrawerSDL::update_state(const ServerMessageDTO& msg, int iterations_ahead) {
    if (msg.type != MsgType::STATE_UPDATE) {
        return;
    }

    const State& state = msg.state;

    std::vector<CarState> predicted_cars;
    CarState client_car;

    for (const auto& car: state.cars) {
        CarState predicted_car = car;
        // Calcular posición futura basada en la velocidad y las iteraciones adelantadas
        float dx = car.speed * iterations_ahead * cos(car.angle);
        float dy = car.speed * iterations_ahead * sin(car.angle);

        predicted_car.x += dx;
        predicted_car.y += dy;

        predicted_cars.push_back(predicted_car);
        if (predicted_car.id == client_id) {
            // Centrar la vista en el auto del cliente (dibujar el mapa acorde a la posición del
            // auto)
            map_drawer.draw(1, static_cast<int>(predicted_car.x),
                            static_cast<int>(predicted_car.y));
            // Uso el primer mapa, se debe agregar lógica para elegir el mapa correcto
            client_car = predicted_car;
        }
    }
    for (const auto& car: predicted_cars) {
        float screen_x = car.x - client_car.x + 400;  // Centrar en pantalla (asumiendo 800x600)
        float screen_y = car.y - client_car.y + 300;  // Centrar en pantalla (asumiendo 800x600)
        car_drawer.draw(car, car.id == client_id, screen_x, screen_y);
    }

    // TODO: Falta agregar logica de dibujo de gente
}
