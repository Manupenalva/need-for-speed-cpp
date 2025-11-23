#include "statistics_drawer.h"

StatisticsDrawer::StatisticsDrawer(SDL2pp::Renderer& renderer): renderer(renderer) {}

void StatisticsDrawer::draw(const ServerMessageDTO& msg) {
    renderer.SetDrawColor(0, 0, 0, 180);

    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);
    ConfigReader& config = ConfigReader::get_instance();

    scale_w = static_cast<float>(w) / config.get_window_width();
    scale_h = static_cast<float>(h) / config.get_window_height();
    scale = std::min(scale_w, scale_h);

    Rect_dimensions rect = {0, 0, w, h};

    renderer.FillRect(SDL2pp::Rect(rect.x, rect.y, rect.w, rect.h));

    draw_titles(msg, rect);

    int y = rect.y + scaled_offset(ROW_TEXT_Y_INIT);
    int pos = 1;

    for (const auto& entry: msg.positions) {
        draw_row(pos, y, entry, rect);
        y += scaled_offset(ROW_HEIGHT);
        pos++;
    }
}

void StatisticsDrawer::draw_text(const std::string& text, int x, int y, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Color color(255, 255, 255, 255);
    SDL2pp::Surface surface = font.RenderText_Solid(text, color);
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(x, y, surface.GetWidth(), surface.GetHeight()));
}

void StatisticsDrawer::draw_centered_text(const std::string& text, int col_index,
                                          Rect_dimensions rect, int y, int font_size) {
    int col_w = rect.w / 4;
    int col_x = rect.x + col_index * col_w;

    int size = scaled_font(font_size);
    int text_w = get_text_width(text, size);

    int x = col_x + HALF(col_w - text_w);

    draw_text(text, x, y, size);
}

int StatisticsDrawer::get_text_width(const std::string& text, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Surface surface = font.RenderText_Solid(text, SDL2pp::Color(255, 255, 255));
    return surface.GetWidth();
}

std::string StatisticsDrawer::format_time(float time) {
    int minutes = static_cast<int>(time) / MINUTES;
    float seconds = time - (minutes * MINUTES);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%06.3f", minutes, seconds);
    return std::string(buffer);
}

void StatisticsDrawer::draw_titles(const ServerMessageDTO& msg, Rect_dimensions rect) {
    int title_y = rect.y + scaled_offset(TITLE_TEXT_Y_OFFSET);
    int col_y = rect.y + scaled_offset(COLUMN_TITLE_Y_OFFSET);

    std::string title = (msg.type == MsgType::RACE_POSITIONS) ? TITLE_RACE : TITLE_GAME;

    int size = scaled_font(TITLE_SIZE);
    int text_w = get_text_width(title, size);
    draw_text(title, rect.x + HALF(rect.w - text_w), title_y, size);

    draw_centered_text(POS_TEXT, 0, rect, col_y, COLUMN_TITLE_SIZE);
    draw_centered_text(PLAYERS_TEXT, 1, rect, col_y, COLUMN_TITLE_SIZE);
    draw_centered_text(TIME_TEXT, 2, rect, col_y, COLUMN_TITLE_SIZE);
    draw_centered_text(PENALIZATION_TEXT, 3, rect, col_y, COLUMN_TITLE_SIZE);
}

void StatisticsDrawer::draw_row(int pos, int y, const auto& entry, Rect_dimensions rect) {
    std::string player_name = entry.name;
    float penalization = entry.penalization_time;
    float time_seconds = entry.time + penalization;

    std::string t_s = format_time(time_seconds);
    std::string t_p = format_time(penalization);

    draw_centered_text(std::to_string(pos), 0, rect, y, FONT_SIZE);
    draw_centered_text(player_name, 1, rect, y, FONT_SIZE);
    draw_centered_text(t_s, 2, rect, y, FONT_SIZE);
    draw_centered_text(t_p, 3, rect, y, FONT_SIZE);
}


int StatisticsDrawer::scaled_font(int base_size) {
    return std::max(15, static_cast<int>(base_size * scale));
}

int StatisticsDrawer::scaled_offset(int base_offset) {
    return static_cast<int>(base_offset * scale);
}
