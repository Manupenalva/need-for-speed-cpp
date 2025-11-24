#include "upgradeScreen_drawer.h"

UpgradeScreenDrawer::UpgradeScreenDrawer(SDL2pp::Renderer& renderer,
                                         TextureManager& texture_manager, TextDrawer& text_drawer,
                                         int client_id):
        renderer(renderer),
        texture_manager(texture_manager),
        text_drawer(text_drawer),
        client_id(client_id) {}

void UpgradeScreenDrawer::draw(const ServerMessageDTO& msg) {
    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);

    Rect_dimensions rect = {0, 0, w, h};
    draw_background(rect);

    Rect_dimensions ui_rect = get_upgrade_ui_rect(rect);
    draw_upgrade_ui(msg, ui_rect);
}

void UpgradeScreenDrawer::draw_background(Rect_dimensions rect) {
    Sprite upgrade_screen_sprite = texture_manager.get_upgrade_screen_sprite();

    SDL2pp::Rect dst_rect(rect.x, rect.y, rect.w, rect.h);

    renderer.Copy(upgrade_screen_sprite.texture, upgrade_screen_sprite.src_rect, dst_rect);
}

void UpgradeScreenDrawer::draw_upgrade_ui(const ServerMessageDTO& msg, Rect_dimensions rect) {
    SDL2pp::Rect box_rect(rect.x, rect.y, rect.w, rect.h);

    renderer.SetDrawColor(255, 255, 255, 60);
    renderer.DrawRect(box_rect);

    text_drawer.draw_text(ATRIBUTE_MESSAGE, rect.x + 20, rect.y + 20, 20, FONT_PATH);

    const PlayerState client_car = get_client_car(msg);
    std::vector<Line> lines = {
            {"Velocidad", client_car.car_properties.max_speed, 1},
            {"Aceleracion", client_car.car_properties.acceleration, 2},
            {"Vida", client_car.car_properties.max_health, 3},
            {"Masa", client_car.car_properties.mass, 4},
            {"Control", client_car.car_properties.control, 5},
    };

    int line_height = 40;
    int start_y = rect.y + 60;

    for (size_t i = 0; i < lines.size(); ++i) {
        const Line& line = lines[i];
        std::string line_text = line.atribute + ": " + std::to_string(line.value) + " (Presiona " +
                                std::to_string(line.upgrade_key) + " para mejorar)";
        text_drawer.draw_text(line_text, rect.x + 20, start_y + i * line_height, 24, FONT_PATH);
    }

    text_drawer.draw_text(
            "Proxima penalizacion: " + text_drawer.format_time(client_car.next_penalization_time),
            rect.x + 20, rect.y + rect.h - 40, 20, FONT_PATH);
}

Rect_dimensions UpgradeScreenDrawer::get_upgrade_ui_rect(Rect_dimensions screen_rect) {
    Rect_dimensions ui_rect;
    ui_rect.w = HALF(screen_rect.w);
    ui_rect.h = HALF(screen_rect.h);
    ui_rect.x = HALF(screen_rect.w - ui_rect.w);
    ui_rect.y = HALF(screen_rect.h - ui_rect.h);
    return ui_rect;
}

PlayerState UpgradeScreenDrawer::get_client_car(const ServerMessageDTO& msg) {
    std::vector<PlayerState> player_states = msg.interval_state.player_states;
    auto it = std::find_if(player_states.begin(), player_states.end(),
                           [this](const PlayerState& player_state) {
                               return player_state.player_id == client_id;
                           });
    if (it != player_states.end()) {
        return *it;
    }
    throw std::runtime_error("Client car properties not found in message.");
}
