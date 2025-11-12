#ifndef FIRE_SHEET_H
#define FIRE_SHEET_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../sprite.h"

#include "animation_sheet.h"

const char* PATH_FIRE = "../client/assets/animations/fuego.png";

#define FIRE_TYPES_PER_ROW 5
#define FIRE_TYPES_PER_COL 3
#define WIDTH_IMAGE 260
#define HEIGHT_IMAGE 260

class FireSheet: public AnimationSheet {
public:
    explicit FireSheet(SDL2pp::Renderer& renderer): AnimationSheet(renderer, PATH_FIRE) {
        width_image = WIDTH_IMAGE;
        height_image = HEIGHT_IMAGE;
        types_per_row = FIRE_TYPES_PER_ROW;
        types_per_col = FIRE_TYPES_PER_COL;
    }
};

#endif
