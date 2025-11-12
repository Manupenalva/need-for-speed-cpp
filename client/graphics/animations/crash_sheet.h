#ifndef CRASH_SHEET_H
#define CRASH_SHEET_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../sprite.h"

#include "animation_sheet.h"

const char* PATH_CRASH = "../client/assets/animations/choque.png";

#define CRASH_TYPES_PER_ROW 4
#define CRASH_TYPES_PER_COL 2
#define WIDTH_IMAGE 519
#define HEIGHT_IMAGE 259

class CrashSheet: public AnimationSheet {
public:
    explicit CrashSheet(SDL2pp::Renderer& renderer): AnimationSheet(renderer, PATH_CRASH) {
        width_image = WIDTH_IMAGE;
        height_image = HEIGHT_IMAGE;
        types_per_row = CRASH_TYPES_PER_ROW;
        types_per_col = CRASH_TYPES_PER_COL;
    }
};

#endif
