#ifndef STATISTICS_DRAWER_H
#define STATISTICS_DRAWER_H

#include <algorithm>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../../common/messageDTOs.h"
#include "../../config/config_reader.h"
#include "../../config/constants.h"

#define POS_TEXT "POSICION"
#define PLAYERS_TEXT "JUGADOR"
#define TIME_TEXT "TIEMPO"
#define PENALIZATION_TEXT "PENALIZACION"

#define TITLE_RACE "ESTADISTICAS DE CARRERA"
#define TITLE_GAME "ESTADISTICAS DEL JUEGO"

#define MINUTES 60

#define TITLE_SIZE 32
#define FONT_SIZE 24
#define COLUMN_TITLE_SIZE 26
#define ROW_HEIGHT 60

#define TITLE_TEXT_Y_OFFSET 20
#define COLUMN_TITLE_Y_OFFSET 80
#define ROW_TEXT_Y_INIT 150

struct Rect_dimensions {
    int x;
    int y;
    int w;
    int h;
};

class StatisticsDrawer {
private:
    SDL2pp::Renderer& renderer;
    float scale_w = 1.0f;
    float scale_h = 1.0f;
    float scale = 1.0f;

    void draw_text(const std::string& text, int x, int y, int size);
    void draw_centered_text(const std::string& text, int col_index, Rect_dimensions rect, int y,
                            int font_size);
    int get_text_width(const std::string& text, int size);

    std::string format_time(float time);
    void draw_titles(const ServerMessageDTO& msg, Rect_dimensions rect);
    void draw_row(int pos, int y, const auto& entry, Rect_dimensions rect);

    int scaled_font(int base_size);
    int scaled_offset(int base_offset);

public:
    explicit StatisticsDrawer(SDL2pp::Renderer& renderer);

    void draw(const ServerMessageDTO& msg);
};

#endif
