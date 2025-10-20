#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>
#include "../common/carstate.h"
#include "../graphics/texture_manager.h"

class CarDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager)
        : renderer(renderer), texture_manager(texture_manager) {}

    void draw(const CarState& car) {
        // Lógica para dibujar el coche
    }
};

#endif