#ifndef FIRE_DRAWER_H
#define FIRE_DRAWER_H

#include <algorithm>
#include <vector>

#include "../drawer.h"

#define AMOUNT_OF_FIRES 15

struct FireInstance {
    float x;
    float y;
    int current_frame;
    int tick_count;
    bool active;

    FireInstance(float x, float y): x(x), y(y), current_frame(0), tick_count(0), active(true) {}
};

class FireDrawer: public Drawer {
private:
    std::vector<FireInstance> fires;
    int frame_duration = 10;  // Duración de cada frame en ticks

    void check_new_fires(const RenderedState& rendered_state);
    void update_fires(const RenderedState& rendered_state);
    void clear_inactive_fires();

public:
    explicit FireDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
