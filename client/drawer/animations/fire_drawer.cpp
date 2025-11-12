#include "fire_drawer.h"

FireDrawer::FireDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void FireDrawer::draw(const RenderedState& rendered_state) {
    check_new_fires(rendered_state);
    update_fires();
    clear_inactive_fires();
}

void FireDrawer::check_new_fires(const RenderedState& rendered_state) {
    std::vector<CarState> cars = rendered_state.state.cars;

    for (const auto& car: cars) {
        if (car.crashed) {
            bool is_active =
                    std::any_of(fires.begin(), fires.end(), [&car](const FireInstance& fire) {
                        return fire.active && (fire.x == car.x && fire.y == car.y);
                    });
            if (!is_active) {
                fires.emplace_back(car.x, car.y);
            }
        }
    }
}

void FireDrawer::update_fires() {
    for (auto& fire: fires) {
        if (fire.active) {
            fire.tick_count++;
            if (fire.tick_count >= frame_duration) {
                fire.tick_count = 0;
                fire.current_frame++;
                if (fire.current_frame >= AMOUNT_OF_FIRES) {
                    fire.active = false;  // Desactivar el fuego después de la última frame
                }
            }

            Sprite fire_sprite = texture_manager.get_fire_sprite(fire.current_frame);
            SDL2pp::Rect dst_rect(static_cast<int>(fire.x) - fire_sprite.src_rect.w / 2,
                                  static_cast<int>(fire.y) - fire_sprite.src_rect.h / 2,
                                  fire_sprite.src_rect.w, fire_sprite.src_rect.h);

            renderer.Copy(fire_sprite.texture, fire_sprite.src_rect, dst_rect);
        }
    }
}

void FireDrawer::clear_inactive_fires() {
    fires.erase(std::remove_if(fires.begin(), fires.end(),
                               [](const FireInstance& fire) { return !fire.active; }),
                fires.end());
}
