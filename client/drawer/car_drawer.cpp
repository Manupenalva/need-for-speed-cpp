#include "car_drawer.h"


CarDrawer::CarDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        renderer(renderer), texture_manager(texture_manager) {}


void CarDrawer::draw(const CarState& car, float screen_x, float screen_y) {
    // Lógica para dibujar el coche
    sprite car_sprite = texture_manager.get_car_sprite(
            ConfigReader::get_instance().get_player_car(),
            car.angle);  // tipo de auto segun el auto elegido por el cliente
    SDL2pp::Rect dst_rect(static_cast<int>(screen_x) - HALF(car_sprite.src_rect.w),
                          static_cast<int>(screen_y) - HALF(car_sprite.src_rect.h),
                          car_sprite.src_rect.w, car_sprite.src_rect.h);
    renderer.Copy(car_sprite.texture, car_sprite.src_rect, dst_rect);
}
