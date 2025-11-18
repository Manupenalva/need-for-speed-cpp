#include "estadistics_drawer.h"

EstadisticsDrawer::EstadisticsDrawer(SDL2pp::Renderer& renderer): renderer(renderer) {}

void EstadisticsDrawer::draw(const ServerMessageDTO& msg) {
    auto positions = msg.positions;

    renderer.SetDrawColor(0, 0, 0, 180);
    ConfigReader& config = ConfigReader::get_instance();
    int rect_x = HALF(config.get_window_width() - RECT_W);
    int rect_y = HALF(config.get_window_height() - RECT_H);
    renderer.FillRect(SDL2pp::Rect(rect_x, rect_y, RECT_W, RECT_H));

    draw_titles(msg, rect_x, rect_y);

    int y = rect_y + ROW_TEXT_Y_INIT;
    int pos = 1;

    for (const auto& entry: positions) {
        draw_row(pos, y, entry, rect_x);
        y += ROW_HEIGHT;
        pos++;
    }
}

void EstadisticsDrawer::draw_text(const std::string& text, int x, int y, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Color color(255, 255, 255, 255);
    SDL2pp::Surface surface = font.RenderText_Solid(text, color);
    SDL2pp::Texture texture(renderer, surface);
    renderer.Copy(texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(x, y, surface.GetWidth(), surface.GetHeight()));
}

std::string EstadisticsDrawer::format_time(float time) {
    int minutes = static_cast<int>(time) / MINUTES;
    float seconds = time - (minutes * MINUTES);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%06.3f", minutes, seconds);
    return std::string(buffer);
}

void EstadisticsDrawer::draw_titles(const ServerMessageDTO& msg, int rect_x, int rect_y) {
    int col_pos_x = rect_x + RECT_W * POS_COL_X_RATIO;
    int col_player_x = rect_x + RECT_W * PLAYER_COL_X_RATIO;
    int col_time_x = rect_x + RECT_W * TIME_COL_X_RATIO;

    int title_y = rect_y + TITLE_TEXT_Y_OFFSET;
    int column_title_y = rect_y + COLUMN_TITLE_Y_OFFSET;

    std::string time_text;
    std::string title;

    if (msg.type == MsgType::RACE_POSITIONS) {
        time_text = TIME_TEXT_RACE;
        title = TITLE_RACE;
    } else {
        time_text = TIME_TEXT_TOTAL;
        title = TITLE_GAME;
    }

    int text_w = get_text_width(title, TITLE_SIZE);
    draw_text(title, rect_x + HALF(RECT_W - text_w), title_y, TITLE_SIZE);

    draw_text(POS_TEXT, col_pos_x, column_title_y, COLUMN_TITLE_SIZE);
    draw_text(PLAYERS_TEXT, col_player_x, column_title_y, COLUMN_TITLE_SIZE);
    draw_text(time_text, col_time_x, column_title_y, COLUMN_TITLE_SIZE);
}

void EstadisticsDrawer::draw_row(int pos, int y, const auto& entry, int rect_x) {
    int col_pos_x = rect_x + RECT_W * POS_COL_X_RATIO;
    int col_player_x = rect_x + RECT_W * PLAYER_COL_X_RATIO;
    int col_time_x = rect_x + RECT_W * TIME_COL_X_RATIO;

    uint16_t player_id = entry.first;
    float time_seconds = entry.second;

    std::string t = format_time(time_seconds);

    draw_text(std::to_string(pos), col_pos_x, y, FONT_SIZE);
    draw_text(std::to_string(player_id), col_player_x, y, FONT_SIZE);
    draw_text(t, col_time_x, y, FONT_SIZE);
}

int EstadisticsDrawer::get_text_width(const std::string& text, int size) {
    SDL2pp::Font font(FONT_PATH, size);
    SDL2pp::Surface surface = font.RenderText_Solid(text, SDL2pp::Color(255, 255, 255));
    return surface.GetWidth();
}
