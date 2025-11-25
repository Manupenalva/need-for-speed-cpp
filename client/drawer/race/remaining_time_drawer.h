#ifndef REMAINING_TIME_DRAWER_H
#define REMAINING_TIME_DRAWER_H

#include <algorithm>
#include <string>

#include "../../config/config_reader.h"
#include "../../config/constants.h"
#include "../drawer.h"
#include "../text/text_drawer.h"

#define TIME_FONT_SIZE 28
#define INITIAL_MARGIN 20

class RemainingTimeDrawer: public Drawer {
private:
    TextDrawer text_drawer;

public:
    explicit RemainingTimeDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
