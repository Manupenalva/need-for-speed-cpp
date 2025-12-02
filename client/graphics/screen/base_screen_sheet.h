#ifndef BASE_SCREEN_SHEET_H
#define BASE_SCREEN_SHEET_H

#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../sprite.h"

class BaseScreenSheet {
private:
    SDL2pp::Renderer& renderer;
    std::unique_ptr<SDL2pp::Texture> texture;

protected:
    std::string screen_path;

public:
    explicit BaseScreenSheet(SDL2pp::Renderer& renderer);

    void load_sprite();

    Sprite get_screen_sprite();
};

#endif
