#ifndef PEOPLE_DRAWER_H
#define PEOPLE_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

class PeopleDrawer {
private:
    SDL2pp::Renderer& renderer;
public:
    explicit PeopleDrawer(SDL2pp::Renderer& renderer) : renderer(renderer) {}

    void draw() {
        // Lógica para dibujar a las personas
    }
};

#endif