#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../config/constants.h"
#include "../drawer.h"

#define NORMAL_OPACITY 255
#define UNDER_BRIDGE_OPACITY 128

class CarDrawer: public Drawer {
private:
    CarState calculate_position(const CarState& car, const int iterations_ahead);
    void draw_car(const CarState& car, float screen_x, float screen_y);

    void draw_npcs(const RenderedState& rendered_state);
    void draw_clients_cars(const RenderedState& rendered_state);

public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
