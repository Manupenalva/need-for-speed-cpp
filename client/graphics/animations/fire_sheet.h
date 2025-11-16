#ifndef FIRE_SHEET_H
#define FIRE_SHEET_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../base_sheet.h"
#include "../sprite.h"

#define FIRE_TYPES_PER_ROW 5
#define FIRE_TYPES_PER_COL 3
#define FIRE_WIDTH_IMAGE 260
#define FIRE_HEIGHT_IMAGE 265

class FireSheet: public BaseSheet {
public:
    explicit FireSheet(SDL2pp::Renderer& renderer): BaseSheet(renderer, PATH_FIRE) {
        width_image = FIRE_WIDTH_IMAGE;
        height_image = FIRE_HEIGHT_IMAGE;
        types_per_row = FIRE_TYPES_PER_ROW;
        types_per_col = FIRE_TYPES_PER_COL;
    }
};

#endif
