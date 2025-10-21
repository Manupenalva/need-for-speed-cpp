#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

struct sprite {
    SDL2pp::Texture& texture;
    SDL2pp::Rect src_rect;
};

#endif
