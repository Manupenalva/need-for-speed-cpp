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
#include "../../graphics/texture_manager.h"
#include "../text/text_drawer.h"

#define POS_TEXT "POSICION"
#define PLAYERS_TEXT "JUGADOR"
#define TIME_TEXT "TIEMPO"
#define PENALIZATION_TEXT "PENALIZACION"

#define TITLE_RACE "ESTADISTICAS DE CARRERA"
#define TITLE_GAME "ESTADISTICAS DE TORNEO"

#define TITLE_SIZE 32
#define FONT_SIZE 24
#define COLUMN_TITLE_SIZE 26
#define ROW_HEIGHT 60

#define TITLE_TEXT_Y_OFFSET 20
#define COLUMN_TITLE_Y_OFFSET 80
#define ROW_TEXT_Y_INIT 150

#define AMOUNT_OF_COLUMNS 4

#define ROW_X_OFFSET 50
#define ROW_Y_OFFSET 10
#define ROW_W_OFFSET 80
#define ROW_H_OFFSET 2


#define GOLD_COLOR() 255, 215, 10, 10
#define SILVER_COLOR() 192, 192, 192, 10
#define BRONZE_COLOR() 205, 127, 50, 10


class StatisticsDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    TextDrawer& text_drawer;

    float scale_w = 1.0f;
    float scale_h = 1.0f;
    float scale = 1.0f;

    void draw_titles(const ServerMessageDTO& msg, Rect_dimensions rect);
    void draw_row(int pos, int y, const auto& entry, Rect_dimensions rect);
    void draw_row_separator(int y, Rect_dimensions rect);
    void paint_first_places(int pos, int y, Rect_dimensions rect);

    int scaled_offset(int base_offset);

public:
    StatisticsDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
                     TextDrawer& text_drawer);

    void draw(const ServerMessageDTO& msg);
};

#endif
