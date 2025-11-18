#ifndef FIRE_DRAWER_H
#define FIRE_DRAWER_H

#include <algorithm>
#include <vector>

#include "animations_drawer.h"

#define AMOUNT_OF_FIRES 15
#define FIRE_DURATION 10

class FireDrawer: public AnimationsDrawer {
public:
    FireDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            AnimationsDrawer(
                    renderer, texture_manager,
                    [](const CarState& car_state) {
                        return car_state.crashed;  // Condición para activar el fuego: si el auto
                                                   // está chocado
                    },
                    [](TextureManager& texture_manager, int frame) {
                        return texture_manager.get_fire_sprite(
                                frame);  // Obtener el sprite de fuego correspondiente al frame
                    },
                    AMOUNT_OF_FIRES, FIRE_DURATION) {}
};

#endif
