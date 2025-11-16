#ifndef COUNTDOWN_DRAWER_H
#define COUNTDOWN_DRAWER_H

#include "../../config/config_reader.h"
#include "../drawer.h"

#define MAX_NUMBER 10

class CountdownDrawer: public Drawer {
public:
    explicit CountdownDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
