#ifndef CAR_DRAWER_H
#define CAR_DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../common/carState.h"
#include "../graphics/sprite.h"
#include "../graphics/texture_manager.h"

class CarDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            renderer(renderer), texture_manager(texture_manager) {}

    void draw(const CarState& car, float screen_x, float screen_y) {
        // Lógica para dibujar el coche
        sprite car_sprite = texture_manager.get_car_sprite(
                1, car.angle);  // tipo de auto segun el auto elegido por el cliente
        SDL2pp::Rect dst_rect(static_cast<int>(screen_x) - car_sprite.src_rect.w / 2,
                              static_cast<int>(screen_y) - car_sprite.src_rect.h / 2,
                              car_sprite.src_rect.w, car_sprite.src_rect.h);
        renderer.Copy(car_sprite.texture, car_sprite.src_rect, dst_rect);
    }
};

#endif
