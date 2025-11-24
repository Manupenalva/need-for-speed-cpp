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
#include "health/health_drawer.h"
#include "map/map_drawer.h"
#include "minimap/minimap_drawer.h"
#include "race/arrow_drawer.h"
#include "race/checkpoint_drawer.h"
#include "screen/upgradeScreen_drawer.h"
#include "statistics/statistics_drawer.h"

#include "drawer.h"
#include "renderedState.h"

class DrawerSDL {
private:
    int client_id;
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    TextDrawer text_drawer;
    std::vector<std::unique_ptr<Drawer>> drawers;
    UpgradeScreenDrawer upgrade_screen_drawer;
    StatisticsDrawer statistics_drawer;

public:
    explicit DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id);

    // Actualiza la vista según el estado recibido de la carrera
    void update_game_state(const ServerMessageDTO& msg, int iterations_ahead, MapType map_id);

    // Actualiza la vista de la pantalla de estadísticas
    void update_statistics_screen(const ServerMessageDTO& msg);

    // Muestra en pantalla la screen para mejorar el auto
    void show_upgrade_screen(const ServerMessageDTO& msg);
};

#endif
