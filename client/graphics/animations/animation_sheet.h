#ifndef ANIMATION_SHEET_H
#define ANIMATION_SHEET_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../sprite.h"

class AnimationSheet {
private:
    SDL2pp::Texture texture;
    std::unordered_map<int, Sprite> animation_sprites;

protected:
    int width_image = 0;
    int height_image = 0;
    int types_per_row = 0;
    int types_per_col = 0;

public:
    AnimationSheet(SDL2pp::Renderer& renderer, const std::string& path);

    void load_sprites();

    Sprite get_sprite(int animation_type);
};
#endif
