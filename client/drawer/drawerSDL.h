#ifndef DRAWERSDL_H
#define DRAWERSDL_H

#include <memory>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"
#include "../config/constants.h"
#include "../graphics/texture_manager.h"

#include "car_drawer.h"
#include "drawer.h"
#include "map_drawer.h"
#include "renderedState.h"

class DrawerSDL {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    std::vector<std::unique_ptr<Drawer>> drawers;

public:
    explicit DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    // Actualiza la vista según el estado recibido
    void update_state(const ServerMessageDTO& msg, int iterations_ahead);
};

#endif
