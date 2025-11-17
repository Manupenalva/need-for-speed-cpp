#ifndef ESTADISTICS_DRAWER_H
#define ESTADISTICS_DRAWER_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../../common/messageDTOs.h"
#include "../../config/config_reader.h"
#include "../../config/constants.h"

#define POS_TEXT "POSICION"
#define PLAYERS_TEXT "JUGADOR"
#define TIME_TEXT_RACE "TIEMPO"
#define TIME_TEXT_TOTAL "TIEMPO ACUMULADO"

#define TITLE_RACE "ESTADISTICAS DE CARRERA"
#define TITLE_GAME "ESTADISTICAS DEL JUEGO"

#define RECT_W 600
#define RECT_H 400

#define MINUTES 60

#define TITLE_SIZE 32
#define FONT_SIZE 24
#define COLUMN_TITLE_SIZE 26
#define ROW_HEIGHT 40

#define POS_COL_X_RATIO 0.10
#define PLAYER_COL_X_RATIO 0.40
#define TIME_COL_X_RATIO 0.75

#define TITLE_TEXT_Y_OFFSET 20
#define COLUMN_TITLE_Y_OFFSET 80

#define ROW_TEXT_Y_INIT 150

class EstadisticsDrawer {
private:
    SDL2pp::Renderer& renderer;

    void draw_text(const std::string& text, int x, int y, int size);
    std::string format_time(float time);
    void draw_titles(const ServerMessageDTO& msg, int rect_x, int rect_y);
    void draw_row(int pos, int y, const auto& entry, int rect_x);
    int get_text_width(const std::string& text, int size);

public:
    explicit EstadisticsDrawer(SDL2pp::Renderer& renderer);

    void draw(const ServerMessageDTO& msg);
};

#endif
