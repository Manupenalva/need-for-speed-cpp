#ifndef DRAWERSDL_H
#define DRAWERSDL_H

#include <SDL2pp/SDL2pp.hh>
#include "../common/messageDTOs.h"
#include "../graphics/texture_manager.h"
#include "car_drawer.h"
#include "people_drawer.h"
#include "map_drawer.h"

class DrawerSDL{
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    CarDrawer car_drawer;
    PeopleDrawer people_drawer;
    MapDrawer map_drawer;

public:
    explicit DrawerSDL(SDL2pp::Renderer& renderer, TextureManager& texture_manager);
    
    // Actualiza la vista según el estado recibido
    void update_state(const ServerMessageDTO& msg, int iterations_ahead);
};

#endif