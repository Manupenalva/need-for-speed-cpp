#ifndef UPGRADE_SCREEN_SHEET_H
#define UPGRADE_SCREEN_SHEET_H

#include "base_screen_sheet.h"

class UpgradeScreenSheet: public BaseScreenSheet {
public:
    explicit UpgradeScreenSheet(SDL2pp::Renderer& renderer): BaseScreenSheet(renderer) {
        this->screen_path = UPGRADE_SCREEN_PATH;
    }
};

#endif
