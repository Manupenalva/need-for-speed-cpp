#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"
#include "../../common/state.h"
#include "../graphics/sprite.h"
#include "../graphics/texture_manager.h"

#include "renderedState.h"

struct Scale {
    float x;
    float y;
};

class Drawer {
protected:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

    explicit Drawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            renderer(renderer), texture_manager(texture_manager) {}

    Scale get_window_scale_factor() {
        int w, h;
        SDL_GetRendererOutputSize(renderer.Get(), &w, &h);
        ConfigReader& config = ConfigReader::get_instance();
        float scale_x = static_cast<float>(w) / config.get_window_width();
        float scale_y = static_cast<float>(h) / config.get_window_height();
        return {scale_x, scale_y};
    }

public:
    virtual ~Drawer() = default;
    virtual void draw(RenderedState& rendered_state) = 0;
};

#endif
