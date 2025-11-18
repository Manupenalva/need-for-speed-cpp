#ifndef HEALTH_DRAWER_H
#define HEALTH_DRAWER_H

#include "../../config/config_reader.h"
#include "../drawer.h"

#define BAR_W 200
#define BAR_H 20
#define BAR_X 30
#define BAR_Y_OFFSET 30

class HealthDrawer: public Drawer {
public:
    explicit HealthDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
