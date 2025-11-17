#ifndef DRAWERSDL_H
#define DRAWERSDL_H

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"
#include "../config/constants.h"
#include "../graphics/texture_manager.h"
#include "animations/burst_drawer.h"
#include "animations/fire_drawer.h"
#include "car/car_drawer.h"
#include "countdown/countdown_drawer.h"
#include "map/map_drawer.h"
#include "minimap/minimap_drawer.h"
#include "race/arrow_drawer.h"
#include "race/checkpoint_drawer.h"
#include "screen/upgradeScreen_drawer.h"

#include "drawer.h"
#include "renderedState.h"

#define POS_TEXT "POSICION"
#define PLAYERS_TEXT "JUGADOR"
#define TIME_TEXT_RACE "TIEMPO"
#define TIME_TEXT_TOTAL "TIEMPO ACUMULADO"

#define TITLE_RACE "ESTADISTICAS DE CARRERA"
#define TITLE_GAME "ESTADISTICAS DEL JUEGO"

#define RECT_X 200
#define RECT_Y 100
#define RECT_W 400
#define RECT_H 300

#define TITLE_SIZE 28
#define TITLE_X_POS 260
#define TITLE_Y_POS 110

#define MINUTES 60

#define COLUMN_TITLE_SIZE 26
#define COLUMN_TITLE_Y_POS 130

#define INITIAL_Y_POS 170
#define ROW_HEIGHT 40

#define COLUMN_POS_X 240
#define COLUMN_PLAYER_X 330
#define COLUMN_TIME_X 600

#define FONT_SIZE 24

class DrawerSDL {
private:
    int client_id;
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    std::vector<std::unique_ptr<Drawer>> drawers;
    UpgradeScreenDrawer upgrade_screen_drawer;

    void draw_text(const std::string& text, int x, int y, int size);
    std::string format_time(float time);
    void draw_titles(const ServerMessageDTO& msg);
    void draw_row(int pos, int y, const auto& entry);

public:
    explicit DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id);

    // Actualiza la vista según el estado recibido de la carrera
    void update_game_state(const ServerMessageDTO& msg, int iterations_ahead, MapType map_id);

    // Actualiza la vista de la pantalla de estadísticas
    void update_estadistics_screen(const ServerMessageDTO& msg);

    // Muestra en pantalla la screen para mejorar el auto
    void show_upgrade_screen();
};

#endif
