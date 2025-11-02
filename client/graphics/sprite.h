#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

struct Sprite {
    SDL2pp::Texture& texture;
    SDL2pp::Rect src_rect;
};

struct Sprite_rotation {
    Sprite sprite;
    float rotation;
};

#endif
