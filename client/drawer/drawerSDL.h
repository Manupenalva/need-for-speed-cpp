#ifndef DRAWERSDL_H
#define DRAWERSDL_H

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"
#include "../config/constants.h"
#include "../graphics/texture_manager.h"
#include "animations/fire_drawer.h"

#include "arrow_drawer.h"
#include "car_drawer.h"
#include "checkpoint_drawer.h"
#include "drawer.h"
#include "map_drawer.h"
#include "renderedState.h"
#include "upgradeScreen_drawer.h"

class DrawerSDL {
private:
    int client_id;
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    std::vector<std::unique_ptr<Drawer>> drawers;
    UpgradeScreenDrawer upgrade_screen_drawer;

public:
    explicit DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager, int client_id);

    // Actualiza la vista según el estado recibido de la carrera
    void update_game_state(const ServerMessageDTO& msg, int iterations_ahead);

    // Muestra en pantalla la screen para mejorar el auto
    void show_upgrade_screen();
};

#endif
