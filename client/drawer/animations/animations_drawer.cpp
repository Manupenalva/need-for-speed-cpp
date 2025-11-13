#include "animations_drawer.h"

AnimationsDrawer::AnimationsDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
                                   std::function<bool(const CarState&)> trigger_condition,
                                   std::function<Sprite(TextureManager&, int)> get_sprite_function,
                                   int amount_of_frames, int frame_duration):
        Drawer(renderer, texture_manager),
        trigger_condition(std::move(trigger_condition)),
        get_sprite_function(std::move(get_sprite_function)),
        amount_of_frames(amount_of_frames),
        frame_duration(frame_duration) {}


void AnimationsDrawer::draw(const RenderedState& rendered_state) {
    check_new_instances(rendered_state);
    update_instances(rendered_state);
    clear_inactive_instances();
}

void AnimationsDrawer::check_new_instances(const RenderedState& rendered_state) {
    std::vector<CarState> cars = rendered_state.state.cars;

    for (const auto& car: cars) {
        if (trigger_condition(car)) {
            bool is_active = std::any_of(animation_instances.begin(), animation_instances.end(),
                                         [&car](const AnimationInstance& instance) {
                                             return instance.active &&
                                                    (instance.x == car.x && instance.y == car.y);
                                         });
            if (!is_active) {
                animation_instances.emplace_back(car.x, car.y);
            }
        }
    }
}

void AnimationsDrawer::update_instances(const RenderedState& rendered_state) {
    for (auto& instance: animation_instances) {
        if (instance.active) {
            instance.tick_count++;
            if (instance.tick_count >= frame_duration) {
                instance.tick_count = 0;
                instance.current_frame++;
                if (instance.current_frame >= amount_of_frames) {
                    instance.active = false;  // Desactivar el fuego después de la última frame
                    continue;
                }
            }

            Sprite fire_sprite = get_sprite_function(texture_manager, instance.current_frame);

            int screen_x = static_cast<int>(instance.x) - rendered_state.map_sprite.src_rect.x;
            int screen_y = static_cast<int>(instance.y) - rendered_state.map_sprite.src_rect.y;

            SDL2pp::Rect dst_rect(screen_x, screen_y, X_DIM, Y_DIM);

            renderer.Copy(fire_sprite.texture, fire_sprite.src_rect, dst_rect);
        }
    }
}

void AnimationsDrawer::clear_inactive_instances() {
    animation_instances.erase(
            std::remove_if(animation_instances.begin(), animation_instances.end(),
                           [](const AnimationInstance& instance) { return !instance.active; }),
            animation_instances.end());
}
