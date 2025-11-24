#ifndef UPGRADE_SCREEN_DRAWER_H
#define UPGRADE_SCREEN_DRAWER_H

#include <string>
#include <vector>

#include "../../../common/messageDTOs.h"
#include "../../config/constants.h"
#include "../../graphics/texture_manager.h"
#include "../drawer.h"
#include "../text/text_drawer.h"

#define ATRIBUTE_MESSAGE "Cada atributo puede ser mejorado una sola vez por ronda"
#define MINUTES 60

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

    void draw_background(Rect_dimensions rect);
    void draw_upgrade_ui(const ServerMessageDTO& msg, Rect_dimensions rect);
    Rect_dimensions get_upgrade_ui_rect(Rect_dimensions screen_rect);
    PlayerState get_client_car(const ServerMessageDTO& msg);

public:
    UpgradeScreenDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
                        TextDrawer& text_drawer, int client_id);

    void draw(const ServerMessageDTO& msg);
};

#endif
