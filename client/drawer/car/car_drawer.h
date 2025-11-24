#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../drawer.h"

#define NORMAL_OPACITY 255
#define UNDER_BRIDGE_OPACITY 128

struct CarScreenPos {
    float x;
    float y;
    float scale_x;
    float scale_y;
};

class CarDrawer: public Drawer {
private:
    bool is_player = false;
    bool is_client_car = false;

    void draw_npcs(const RenderedState& rendered_state);
    void draw_clients_cars(RenderedState& rendered_state);
    SDL2pp::Rect draw_car(const CarState& car, const CarScreenPos& screen_pos);

    CarState calculate_position(const CarState& car, const int iterations_ahead);
    CarScreenPos calculate_map_scale(const CarState& car, const SDL2pp::Rect& map_rect);

    bool is_drawable(const SDL2pp::Rect& car_rect);
    void appply_visual_effects(const CarState& car, const CarScreenPos& screen_pos,
                               Sprite& car_sprite);
    void draw_border(const CarState& player_car, int screen_x, int screen_y);

public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
