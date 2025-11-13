#ifndef ANIMATIONS_DRAWER_H
#define ANIMATIONS_DRAWER_H

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "../drawer.h"

struct AnimationInstance {
    float x;
    float y;
    int current_frame;
    int tick_count;
    bool active;

    AnimationInstance(float x, float y):
            x(x), y(y), current_frame(0), tick_count(0), active(true) {}
};

#define X_DIM 40
#define Y_DIM 40

class AnimationsDrawer: public Drawer {
protected:
    std::vector<AnimationInstance> animation_instances;
    std::function<bool(const CarState&)>
            trigger_condition;  // Función para chequear nuevas instancias (si activar fuego o
                                // choque)
    std::function<Sprite(TextureManager&, int)>
            get_sprite_function;  // Función para obtener el sprite correspondiente
    int amount_of_frames;
    int frame_duration;  // Duración de cada frame en ticks

private:
    void check_new_instances(const RenderedState& rendered_state);
    void update_instances(const RenderedState& rendered_state);
    void clear_inactive_instances();

public:
    explicit AnimationsDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
                              std::function<bool(const CarState&)> trigger_condition,
                              std::function<Sprite(TextureManager&, int)> get_sprite_function,
                              int amount_of_frames, int frame_duration);

    void draw(const RenderedState& rendered_state) override;
};

#endif
