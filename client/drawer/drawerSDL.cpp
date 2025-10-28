#include "drawerSDL.h"

#include <utility>
#include <vector>

#include <SDL2/SDL.h>

DrawerSDL::DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        renderer(renderer), texture_manager(texture_manager) {
    drawers.push_back(std::make_unique<MapDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CarDrawer>(renderer, texture_manager));
}

void DrawerSDL::update_state(const ServerMessageDTO& msg, int iterations_ahead) {
    if (msg.type != MsgType::STATE_UPDATE) {
        return;
    }

    const State& state = msg.state;

    // Obtener el sprite del mapa centrado en el auto del cliente
    const CarState& client_car = state.cars[msg.id];
    sprite map_sprite = texture_manager.get_map_sprite(ConfigReader::get_instance().get_map_id(),
                                                       client_car.x, client_car.y);
    RenderedState rendered_state{iterations_ahead, map_sprite, state};

    for (const auto& drawer: drawers) {
        drawer->draw(rendered_state);  // Usa polimorfismo para dibujar
    }
}
