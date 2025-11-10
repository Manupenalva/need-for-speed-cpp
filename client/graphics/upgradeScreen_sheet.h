#ifndef UPGRADE_SCREEN_SHEET_H
#define UPGRADE_SCREEN_SHEET_H

#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../config/constants.h"

#include "sprite.h"

class UpgradeScreenSheet {
private:
    SDL2pp::Renderer& renderer;
    std::unique_ptr<SDL2pp::Texture> texture;
    std::string upgrade_screen_path;

public:
    UpgradeScreenSheet(SDL2pp::Renderer& renderer, const std::string& upgrade_screen_path);

    void load_sprite();

    Sprite get_screen_sprite();
};

#endif
