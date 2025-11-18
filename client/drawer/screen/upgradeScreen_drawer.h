#ifndef UPGRADE_SCREEN_DRAWER_H
#define UPGRADE_SCREEN_DRAWER_H

#include "../drawer.h"

class UpgradeScreenDrawer {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;

public:
    explicit UpgradeScreenDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw();
};

#endif
