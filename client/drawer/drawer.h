#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"
#include "../../common/state.h"
#include "../graphics/sprite.h"
#include "../graphics/texture_manager.h"

#include "renderedState.h"

class Drawer {
protected:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

    explicit Drawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            renderer(renderer), texture_manager(texture_manager) {}

public:
    virtual ~Drawer() = default;
    virtual void draw(const RenderedState& rendered_state) = 0;

    Sprite get_map_sprite(int map_id, float focus_x, float focus_y) {
        return texture_manager.get_map_sprite(map_id, focus_x, focus_y);
    }
};

#endif
