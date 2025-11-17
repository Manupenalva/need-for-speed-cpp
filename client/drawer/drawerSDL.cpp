#include "drawerSDL.h"

DrawerSDL::DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id):
        client_id(client_id),
        renderer(renderer),
        texture_manager(texture_manager),
        upgrade_screen_drawer(renderer, texture_manager) {
    drawers.push_back(std::make_unique<MapDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<ArrowDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CheckpointDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CarDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<FireDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<BurstDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<MinimapDrawer>(renderer, texture_manager));
    drawers.push_back(std::make_unique<CountdownDrawer>(renderer, texture_manager));
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

void DrawerSDL::show_upgrade_screen() { upgrade_screen_drawer.draw(); }

void DrawerSDL::update_estadistics_screen(const ServerMessageDTO& msg) {
    auto positions = msg.positions;

    renderer.SetDrawColor(0, 0, 0, 180);
    renderer.FillRect(SDL2pp::Rect(RECT_X, RECT_Y, RECT_W, RECT_H));

    draw_titles(msg);

    int y = INITIAL_Y_POS;
    int pos = 1;

    for (const auto& entry: positions) {
        draw_row(pos, y, entry);
        y += ROW_HEIGHT;
        pos++;
    }
}

void DrawerSDL::draw_text(const std::string& text, int x, int y, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Color color(255, 255, 255, 255);
    SDL2pp::Surface surface = font.RenderText_Solid(text, color);
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(x, y, surface.GetWidth(), surface.GetHeight()));
}

std::string DrawerSDL::format_time(float time) {
    int minutes = static_cast<int>(time) / MINUTES;
    float seconds = time - (minutes * MINUTES);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%06.3f", minutes, seconds);
    return std::string(buffer);
}

void DrawerSDL::draw_titles(const ServerMessageDTO& msg) {
    std::string time_text;
    std::string title;

    if (msg.type == MsgType::RACE_POSITIONS) {
        time_text = TIME_TEXT_RACE;
        title = TITLE_RACE;
    } else {
        time_text = TIME_TEXT_TOTAL;
        title = TITLE_GAME;
    }

    draw_text(title, TITLE_X_POS, TITLE_Y_POS, TITLE_SIZE);

    draw_text(POS_TEXT, COLUMN_POS_X, COLUMN_TITLE_Y_POS, COLUMN_TITLE_SIZE);
    draw_text(PLAYERS_TEXT, COLUMN_PLAYER_X, COLUMN_TITLE_Y_POS, COLUMN_TITLE_SIZE);
    draw_text(time_text, COLUMN_TIME_X, COLUMN_TITLE_Y_POS, COLUMN_TITLE_SIZE);
}

void DrawerSDL::draw_row(int pos, int y, const auto& entry) {
    uint16_t player_id = entry.first;
    float time_seconds = entry.second;

    std::string t = format_time(time_seconds);

    draw_text(std::to_string(pos), COLUMN_POS_X, y, FONT_SIZE);
    draw_text(std::to_string(player_id), COLUMN_PLAYER_X, y, FONT_SIZE);
    draw_text(t, COLUMN_TIME_X, y, FONT_SIZE);
}
