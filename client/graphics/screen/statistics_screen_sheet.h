#ifndef STATISTICS_SCREEN_SHEET_H
#define STATISTICS_SCREEN_SHEET_H

#include "base_screen_sheet.h"

class StatisticsScreenSheet: public BaseScreenSheet {
public:
    explicit StatisticsScreenSheet(SDL2pp::Renderer& renderer): BaseScreenSheet(renderer) {
        this->screen_path = STATISTICS_SCREEN_PATH;
    }
};

#endif
