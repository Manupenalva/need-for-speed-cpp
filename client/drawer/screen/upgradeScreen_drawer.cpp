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

    ConfigReader& config = ConfigReader::get_instance();
    scale = std::min(w / config.get_window_width(), h / config.get_window_height());
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

    renderer.SetDrawColor(WHITE_COLOR(80));
    for (int i = 0; i < BORDER_LAYERS; ++i) {
        SDL2pp::Rect border(rect.x - i, rect.y - i, rect.w + BORDER_Y_OFFSET * i,
                            rect.h + BORDER_Y_OFFSET * i);  // Dibuja el borde con mayor grosor
        renderer.DrawRect(border);
    }

    std::string title = ATRIBUTE_MESSAGE;
    int title_size = text_drawer.scaled_font(TITLE_SIZE, scale);
    int title_w = text_drawer.get_text_width(title, title_size);
    int title_x = rect.x + HALF(rect.w - title_w);
    int title_y = rect.y + rect.h * TITLE_TOP_MARGIN;

    text_drawer.draw_text(title, title_x + SHADOW_OFFSET, title_y + SHADOW_OFFSET, title_size,
                          FONT_PATH, SDL2pp::Color(0, 0, 0, 150));  // Dibuja sombra de título

    text_drawer.draw_text(title, title_x, title_y, title_size, FONT_PATH,
                          SDL2pp::Color(WHITE_COLOR(255)));  // Dibuja título principal

    const PlayerState client_car = get_client_car(msg);
    draw_atributes(client_car, rect, title_y);

    draw_penalization(client_car, rect);
}

void UpgradeScreenDrawer::draw_atributes(const PlayerState& client_car, Rect_dimensions rect,
                                         int title_y) {

    std::vector<Line> lines = {
            {ATRIBUTTES[0].first, client_car.car_properties.max_speed, ATRIBUTTES[0].second},
            {ATRIBUTTES[1].first, client_car.car_properties.acceleration, ATRIBUTTES[1].second},
            {ATRIBUTTES[2].first, client_car.car_properties.max_health, ATRIBUTTES[2].second},
            {ATRIBUTTES[3].first, client_car.car_properties.mass, ATRIBUTTES[3].second},
            {ATRIBUTTES[4].first, client_car.car_properties.control, ATRIBUTTES[4].second},
    };

    int line_height = rect.h * LINE_HEIGHT_FACTOR;
    int start_y = title_y + rect.h * ATRIBUTES_TOP_MARGIN;

    int label_size = text_drawer.scaled_font(ATRIBUTE_SIZE, scale);

    for (size_t i = 0; i < lines.size(); i++) {
        const auto& s = lines[i];
        int row_y = start_y + i * line_height;

        std::string label = s.atribute + "  (tecla " + std::to_string(s.upgrade_key) + ")";

        text_drawer.draw_text(label, rect.x + rect.w * PADEL_PADDING, row_y, label_size, FONT_PATH,
                              SDL2pp::Color(WHITE_COLOR(180)));

        int bar_h = rect.h * BAR_HEIGHT_FACTOR;
        int bar_w = rect.w * BAR_WIDTH_FACTOR;
        int bar_x = rect.x + rect.w - bar_w - rect.w * BAR_RIGHT_MARGIN;
        int bar_y = row_y + bar_h * ATRIBUTE_BAR_Y_OFFSET;

        renderer.SetDrawColor(WHITE_COLOR(100));
        renderer.FillRect(SDL2pp::Rect(bar_x, bar_y, bar_w, bar_h));

        float pct = std::min(MIN_VALUE_ATRIBUTE, s.value / MAX_VALUE_ATRIBUTE);
        renderer.SetDrawColor(80, 180, 255, 200);
        renderer.FillRect(SDL2pp::Rect(bar_x, bar_y, bar_w * pct, bar_h));
    }
}

void UpgradeScreenDrawer::draw_penalization(const PlayerState& client_car, Rect_dimensions rect) {
    int penalty_size = text_drawer.scaled_font(PENALTY_SIZE, scale);
    text_drawer.draw_text(
            "Proxima penalizacion: " + text_drawer.format_time(client_car.next_penalization_time),
            rect.x + rect.w * PADEL_PADDING, rect.y + rect.h - rect.h * PENALTY_BOTTOM_MARGIN,
            penalty_size, FONT_PATH, SDL2pp::Color(WHITE_COLOR(200)));
}

Rect_dimensions UpgradeScreenDrawer::get_upgrade_ui_rect(Rect_dimensions screen_rect) {
    Rect_dimensions ui_rect;
    ui_rect.w = screen_rect.w * UI_SCALE_FACTOR;
    ui_rect.h = HALF(screen_rect.h) * UI_HEIGHT_SCALE;
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
