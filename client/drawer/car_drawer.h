#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../config/constants.h"

#include "drawer.h"

class CarDrawer: public Drawer {
private:
    CarState calculate_position(const CarState& car, int iterations_ahead);
    void draw_car(const CarState& car, float screen_x, float screen_y);

public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
