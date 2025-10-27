#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../common/carState.h"
#include "../config/config_reader.h"
#include "../config/constants.h"
#include "../graphics/sprite.h"
#include "../graphics/texture_manager.h"

class CarDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const CarState& car, float screen_x, float screen_y);
};

#endif
