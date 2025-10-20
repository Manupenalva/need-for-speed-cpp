#ifndef MAP_DRAWER_H
#define MAP_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

class MapDrawer {
private:
    SDL2pp::Renderer& renderer;
public:
    explicit MapDrawer(SDL2pp::Renderer& renderer) : renderer(renderer) {}

    void draw() {
        // Lógica para dibujar el mapa
    }
};

#endif