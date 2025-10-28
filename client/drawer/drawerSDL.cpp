#include "drawerSDL.h"

DrawerSDL::DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id):
        client_id(client_id), renderer(renderer), texture_manager(texture_manager) {
    drawers.push_back(std::make_unique<MapDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CarDrawer>(renderer, texture_manager));
}

void DrawerSDL::update_state(const ServerMessageDTO& msg, int iterations_ahead) {
    if (msg.type != MsgType::STATE_UPDATE) {
        return;
    }

    const State& state = msg.state;

    auto it = std::find_if(state.cars.begin(), state.cars.end(),
                           [this](const CarState& car) { return car.id == client_id; });

    if (it == state.cars.end()) {
        throw std::runtime_error("Client car not found in state.");
    }

    const CarState& client_car = *it;

    // Obtener el sprite del mapa centrado en el auto del cliente
    sprite map_sprite = texture_manager.get_map_sprite(ConfigReader::get_instance().get_map_id(),
                                                       client_car.x, client_car.y);
    RenderedState rendered_state{iterations_ahead, map_sprite, state};

    for (const auto& drawer: drawers) {
        drawer->draw(rendered_state);  // Usa polimorfismo para dibujar
    }
}
