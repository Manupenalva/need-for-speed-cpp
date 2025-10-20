#ifndef DRAWERSDL_H
#define DRAWERSDL_H

#include <SDL2pp/SDL2pp.hh>
#include "../common/messageDTOs.h"
#include "car_drawer.h"
#include "people_drawer.h"
#include "map_drawer.h"

class DrawerSDL{
private:
    SDL2pp::Renderer& renderer;
    CarDrawer car_drawer;
    PeopleDrawer people_drawer;
    MapDrawer map_drawer;

public:
    explicit DrawerSDL(SDL2pp::Renderer& renderer);
    
    // Actualiza la vista según el estado recibido
    void update_state(const ServerMessageDTO& msg, int iterations_ahead);
};

#endif