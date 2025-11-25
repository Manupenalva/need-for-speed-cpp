#include "statistics_drawer.h"

StatisticsDrawer::StatisticsDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
                                   TextDrawer& text_drawer):
        renderer(renderer), texture_manager(texture_manager), text_drawer(text_drawer) {}

void StatisticsDrawer::draw(const ServerMessageDTO& msg) {
    int w, h;
    SDL_GetRendererOutputSize(renderer.Get(), &w, &h);
    ConfigReader& config = ConfigReader::get_instance();

    scale_w = static_cast<float>(w) / config.get_window_width();
    scale_h = static_cast<float>(h) / config.get_window_height();
    scale = std::min(scale_w, scale_h);

    Rect_dimensions rect = {0, 0, w, h};

    Sprite background = texture_manager.get_statistics_screen_sprite();
    renderer.Copy(background.texture, background.src_rect,
                  SDL2pp::Rect(rect.x, rect.y, rect.w, rect.h));

    draw_titles(msg, rect);

    int y = rect.y + scaled_offset(ROW_TEXT_Y_INIT);
    int pos = 1;

    for (const auto& entry: msg.positions) {
        draw_row(pos, y, entry, rect);
        y += scaled_offset(ROW_HEIGHT);
        pos++;
    }
}

void StatisticsDrawer::draw_titles(const ServerMessageDTO& msg, Rect_dimensions rect) {
    int title_y = rect.y + scaled_offset(TITLE_TEXT_Y_OFFSET);
    int col_y = rect.y + scaled_offset(COLUMN_TITLE_Y_OFFSET);

    std::string title = (msg.type == MsgType::RACE_POSITIONS) ? TITLE_RACE : TITLE_GAME;

    int size = text_drawer.scaled_font(TITLE_SIZE, scale);
    int text_w = text_drawer.get_text_width(title, size);
    text_drawer.draw_text(title, rect.x + HALF(rect.w - text_w), title_y, size, FONT_PATH);

    text_drawer.draw_centered_text(POS_TEXT, 0, rect, col_y, COLUMN_TITLE_SIZE, AMOUNT_OF_COLUMNS,
                                   scale, FONT_PATH);
    text_drawer.draw_centered_text(PLAYERS_TEXT, 1, rect, col_y, COLUMN_TITLE_SIZE,
                                   AMOUNT_OF_COLUMNS, scale, FONT_PATH);
    text_drawer.draw_centered_text(TIME_TEXT, 2, rect, col_y, COLUMN_TITLE_SIZE, AMOUNT_OF_COLUMNS,
                                   scale, FONT_PATH);
    text_drawer.draw_centered_text(PENALIZATION_TEXT, 3, rect, col_y, COLUMN_TITLE_SIZE,
                                   AMOUNT_OF_COLUMNS, scale, FONT_PATH);
}

void StatisticsDrawer::draw_row(int pos, int y, const auto& entry, Rect_dimensions rect) {
    paint_first_places(pos, y, rect);

    std::string player_name = entry.name;
    float penalization = entry.penalization_time;
    float time_seconds = entry.time + penalization;

    std::string t_s = text_drawer.format_time(time_seconds);
    std::string t_p = text_drawer.format_time(penalization);

    text_drawer.draw_centered_text(std::to_string(pos), 0, rect, y, FONT_SIZE, AMOUNT_OF_COLUMNS,
                                   scale, FONT_PATH);
    text_drawer.draw_centered_text(player_name, 1, rect, y, FONT_SIZE, AMOUNT_OF_COLUMNS, scale,
                                   FONT_PATH);
    text_drawer.draw_centered_text(t_s, 2, rect, y, FONT_SIZE, AMOUNT_OF_COLUMNS, scale, FONT_PATH);
    text_drawer.draw_centered_text(t_p, 3, rect, y, FONT_SIZE, AMOUNT_OF_COLUMNS, scale, FONT_PATH);
    draw_row_separator(y, rect);
}

void StatisticsDrawer::paint_first_places(int pos, int y, Rect_dimensions rect) {
    if (pos == 1) {
        renderer.SetDrawColor(GOLD_COLOR());
    } else if (pos == 2) {
        renderer.SetDrawColor(SILVER_COLOR());
    } else if (pos == 3) {
        renderer.SetDrawColor(BRONZE_COLOR());
    } else {
        return;
    }
    renderer.FillRect(
            SDL2pp::Rect(rect.x + scaled_offset(ROW_X_OFFSET), y - scaled_offset(ROW_Y_OFFSET),
                         HALF(rect.w) - scaled_offset(ROW_W_OFFSET), scaled_offset(ROW_HEIGHT)));
}

void StatisticsDrawer::draw_row_separator(int y, Rect_dimensions rect) {
    renderer.SetDrawColor(WHITE_COLOR(255));
    SDL2pp::Rect line_rect(rect.x + scaled_offset(ROW_X_OFFSET), y - scaled_offset(ROW_Y_OFFSET),
                           HALF(rect.w) - scaled_offset(ROW_W_OFFSET), scaled_offset(ROW_H_OFFSET));
    renderer.FillRect(line_rect);
}

int StatisticsDrawer::scaled_offset(int base_offset) {
    return static_cast<int>(base_offset * scale);
}
