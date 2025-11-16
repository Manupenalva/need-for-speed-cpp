#ifndef MINIMAP_SHEET_H
#define MINIMAP_SHEET_H

#include <memory>
#include <string>
#include <utility>

#include "../../../common/minimapInfo.h"
#include "../../config/constants.h"
#include "../race_sheet.h"
#include "../sprite.h"
#include "SDL2pp/SDL2pp.hh"

class MinimapSheet {
private:
    SDL2pp::Renderer& renderer;
    std::unique_ptr<SDL2pp::Texture> texture;
    RaceSheet& race_sheet;

public:
    explicit MinimapSheet(SDL2pp::Renderer& renderer, RaceSheet& race_sheet);

    void load_texture(const MapType map_type);

    SDL2pp::Texture& get_minimap_texture();

    void load_minimap_info(const MinimapInfo& info);
};

#endif
