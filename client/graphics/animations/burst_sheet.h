#ifndef BURST_SHEET_H
#define BURST_SHEET_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../base_sheet.h"
#include "../sprite.h"

#define BURST_TYPES_PER_ROW 4
#define BURST_TYPES_PER_COL 2
#define BURST_WIDTH_IMAGE 520
#define BURST_HEIGHT_IMAGE 260

class BurstSheet: public BaseSheet {
public:
    explicit BurstSheet(SDL2pp::Renderer& renderer): BaseSheet(renderer, PATH_BURST) {
        width_image = BURST_WIDTH_IMAGE;
        height_image = BURST_HEIGHT_IMAGE;
        types_per_row = BURST_TYPES_PER_ROW;
        types_per_col = BURST_TYPES_PER_COL;
    }
};

#endif
