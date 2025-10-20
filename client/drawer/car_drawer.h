#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>
#include "../common/carstate.h"

class CarDrawer {
private:
    SDL2pp::Renderer& renderer;
public:
    explicit CarDrawer(SDL2pp::Renderer& renderer) : renderer(renderer) {}

    void draw(const CarState& car) {
        // Lógica para dibujar el coche
    }
};

#endif