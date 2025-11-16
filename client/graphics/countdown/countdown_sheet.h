#ifndef COUNTDOWN_SHEET_H
#define COUNTDOWN_SHEET_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../base_sheet.h"
#include "../sprite.h"

#define COUNTDOWN_TYPES_PER_ROW 5
#define COUNTDOWN_TYPES_PER_COL 2
#define COUNTDOWN_WIDTH_IMAGE 305
#define COUNTDOWN_HEIGHT_IMAGE 164

class CountdownSheet: public BaseSheet {
public:
    explicit CountdownSheet(SDL2pp::Renderer& renderer): BaseSheet(renderer, PATH_COUNTDOWN) {
        width_image = COUNTDOWN_WIDTH_IMAGE;
        height_image = COUNTDOWN_HEIGHT_IMAGE;
        types_per_row = COUNTDOWN_TYPES_PER_ROW;
        types_per_col = COUNTDOWN_TYPES_PER_COL;
    }
};

#endif
