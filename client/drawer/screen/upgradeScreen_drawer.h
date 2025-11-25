#ifndef UPGRADE_SCREEN_DRAWER_H
#define UPGRADE_SCREEN_DRAWER_H

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../../../common/messageDTOs.h"
#include "../../config/constants.h"
#include "../../graphics/texture_manager.h"
#include "../drawer.h"
#include "../text/text_drawer.h"

#define ATRIBUTE_MESSAGE "Cada atributo puede ser mejorado una sola vez por ronda"
#define TITLE_SIZE 32
#define ATRIBUTE_SIZE 26
#define PENALTY_SIZE 22

const std::array<std::pair<std::string, int>, 5> ATRIBUTTES = {{
        {"Velocidad", 1},
        {"Aceleracion", 2},
        {"Vida", 3},
        {"Masa", 4},
        {"Control", 5},
}};

#define BORDER_LAYERS 4
#define BORDER_Y_OFFSET 2

#define UI_SCALE_FACTOR 0.75f
#define UI_HEIGHT_SCALE 0.8f

#define TITLE_TOP_MARGIN 0.05f

#define ATRIBUTES_TOP_MARGIN 0.10f
#define LINE_HEIGHT_FACTOR 0.12f

#define BAR_HEIGHT_FACTOR 0.018f
#define BAR_WIDTH_FACTOR 0.28f
#define BAR_RIGHT_MARGIN 0.05f
#define ATRIBUTE_BAR_Y_OFFSET 1.8f

#define PENALTY_BOTTOM_MARGIN 0.10f

#define PADEL_PADDING 0.04f

struct Line {
    std::string atribute;
    int value;
    int upgrade_key;
};

class UpgradeScreenDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    TextDrawer& text_drawer;
    int client_id;
    float scale = 1.0f;

    void draw_background(Rect_dimensions rect);
    void draw_upgrade_ui(const ServerMessageDTO& msg, Rect_dimensions rect);
    void draw_atributes(const PlayerState& client_car, Rect_dimensions rect, int title_y);
    void draw_penalization(const PlayerState& client_car, Rect_dimensions rect);
    Rect_dimensions get_upgrade_ui_rect(Rect_dimensions screen_rect);
    PlayerState get_client_car(const ServerMessageDTO& msg);

public:
    UpgradeScreenDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
                        TextDrawer& text_drawer, int client_id);

    void draw(const ServerMessageDTO& msg);
};

#endif
