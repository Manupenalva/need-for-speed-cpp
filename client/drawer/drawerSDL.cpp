#include "drawerSDL.h"
#include <SDL2/SDL.h>
#include <utility>

DrawerSDL::DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager)
    : renderer(renderer),
      texture_manager(texture_manager),
      car_drawer(renderer, texture_manager),
      people_drawer(renderer, texture_manager),
      map_drawer(renderer, texture_manager) {}

DrawerSDL::~DrawerSDL() = default;

void DrawerSDL::update_state(const ServerMessageDTO& msg, int iterations_ahead) {
    if (msg.type != MsgType::STATE_UPDATE) {
        return;
    }

    map_drawer.draw();

    const State& state = msg.state;

    for (const auto& car : state.cars) {
        CarState predicted_car = car;
        // Calcular posición futura basada en la velocidad y las iteraciones adelantadas
        float dx = car.speed * iterations_ahead * cos(car.angle);
        float dy = car.speed * iterations_ahead * sin(car.angle);
        
        predicted_car.x += dx;
        predicted_car.y += dy;

        car_drawer.draw(predicted_car);
    }
    // TODO: Falta agregar logica de dibujo de gente
}

