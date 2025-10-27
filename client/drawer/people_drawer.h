#ifndef PEOPLE_DRAWER_H
#define PEOPLE_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../graphics/texture_manager.h"

class PeopleDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit PeopleDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw();
};

#endif
