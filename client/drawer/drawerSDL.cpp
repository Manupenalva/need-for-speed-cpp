#include "drawerSDL.h"

DrawerSDL::DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id):
        client_id(client_id),
        renderer(renderer),
        texture_manager(texture_manager),
        text_drawer(renderer),
        upgrade_screen_drawer(renderer, texture_manager, text_drawer, client_id),
        statistics_drawer(renderer, texture_manager, text_drawer) {
    drawers.push_back(std::make_unique<MapDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<ArrowDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CheckpointDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CarDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<FireDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<BurstDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<MinimapDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CountdownDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<HealthDrawer>(renderer, texture_manager));
}

void DrawerSDL::update_game_state(const ServerMessageDTO& msg, int iterations_ahead,
                                  MapType map_id) {
    const State& state = msg.state;

    auto it = std::find_if(state.cars.begin(), state.cars.end(),
                           [this](const CarState& car) { return car.id == client_id; });

    if (it == state.cars.end()) {
        throw std::runtime_error("Client car not found in state.");
    }

    const CarState& client_car = *it;

    // Obtener el sprite del mapa centrado en el auto del cliente
    Sprite map_sprite = texture_manager.get_map_sprite(map_id, client_car.x, client_car.y);

    RenderedState rendered_state{iterations_ahead, client_car, map_sprite, state};


    for (const auto& drawer: drawers) {
        drawer->draw(rendered_state);  // Usa polimorfismo para dibujar
    }
}

void DrawerSDL::show_upgrade_screen(const ServerMessageDTO& msg) {
    upgrade_screen_drawer.draw(msg);
}

void DrawerSDL::update_statistics_screen(const ServerMessageDTO& msg) {
    statistics_drawer.draw(msg);
}
