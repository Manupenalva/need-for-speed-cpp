#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../drawer.h"

#define NORMAL_OPACITY 255
#define UNDER_BRIDGE_OPACITY 128

class CarDrawer: public Drawer {
private:
    bool is_player = false;
    bool is_client_car = false;

    CarState calculate_position(const CarState& car, const int iterations_ahead);
    SDL2pp::Rect draw_car(const CarState& car, float screen_x, float screen_y);

    void draw_npcs(const RenderedState& rendered_state);
    void draw_clients_cars(RenderedState& rendered_state);
    void draw_border(const CarState& player_car, int screen_x, int screen_y);

public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
