#include "statistics_drawer.h"

StatisticsDrawer::StatisticsDrawer(SDL2pp::Renderer& renderer): renderer(renderer) {}

void StatisticsDrawer::draw(const ServerMessageDTO& msg) {
    auto positions = msg.positions;

    renderer.SetDrawColor(0, 0, 0, 180);
    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);
    ConfigReader& config = ConfigReader::get_instance();

    scale = std::min(w / config.get_window_width(), h / config.get_window_height());

    Rect_dimensions rect;

    rect.w = static_cast<int>(w * RECT_W_RATIO);
    rect.h = static_cast<int>(h * RECT_H_RATIO);
    rect.x = HALF(w - rect.w);
    rect.y = HALF(h - rect.h);

    renderer.FillRect(SDL2pp::Rect(rect.x, rect.y, rect.w, rect.h));
    draw_titles(msg, rect);

    int y = rect.y + ROW_TEXT_Y_INIT * scale;
    int pos = 1;

    for (const auto& entry: positions) {
        draw_row(pos, y, entry, rect);
        y += ROW_HEIGHT * scale;
        pos++;
    }
}

std::vector<int> StatisticsDrawer::compute_column_x_positions(int rect_x, int rect_w,
                                                              int col_count) {
    std::vector<int> positions;
    int spacing = rect_w / (col_count + 1);
    for (int i = 1; i <= col_count; ++i) {
        positions.push_back(rect_x + i * spacing);
    }
    return positions;
}

void StatisticsDrawer::draw_text(const std::string& text, int x, int y, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Color color(255, 255, 255, 255);
    SDL2pp::Surface surface = font.RenderText_Solid(text, color);
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(x, y, surface.GetWidth(), surface.GetHeight()));
}

std::string StatisticsDrawer::format_time(float time) {
    int minutes = static_cast<int>(time) / MINUTES;
    float seconds = time - (minutes * MINUTES);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%06.3f", minutes, seconds);
    return std::string(buffer);
}

void StatisticsDrawer::draw_titles(const ServerMessageDTO& msg, Rect_dimensions rect) {
    auto positions = compute_column_x_positions(rect.x, rect.w, 3);

    int title_y = rect.y + TITLE_TEXT_Y_OFFSET * scale;
    int col_y = rect.y + COLUMN_TITLE_Y_OFFSET * scale;

    std::string title = (msg.type == MsgType::RACE_POSITIONS) ? TITLE_RACE : TITLE_GAME;

    int text_w = get_text_width(title, TITLE_SIZE * scale);
    draw_text(title, rect.x + HALF(rect.w - text_w), title_y, TITLE_SIZE * scale);

    draw_text(POS_TEXT, positions[0], col_y, COLUMN_TITLE_SIZE * scale);
    draw_text(PLAYERS_TEXT, positions[1], col_y, COLUMN_TITLE_SIZE * scale);
    draw_text(TIME_TEXT, positions[2], col_y, COLUMN_TITLE_SIZE * scale);
}

void StatisticsDrawer::draw_row(int pos, int y, const auto& entry, Rect_dimensions rect) {
    auto positions = compute_column_x_positions(rect.x, rect.w, 3);

    uint16_t player_id = entry.first;
    float time_seconds = entry.second;

    std::string t = format_time(time_seconds);

    draw_text(std::to_string(pos), positions[0], y, FONT_SIZE * scale);
    draw_text(std::to_string(player_id), positions[1], y, FONT_SIZE * scale);
    draw_text(t, positions[2], y, FONT_SIZE * scale);
}

int StatisticsDrawer::get_text_width(const std::string& text, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Surface surface = font.RenderText_Solid(text, SDL2pp::Color(255, 255, 255));
    return surface.GetWidth();
}
