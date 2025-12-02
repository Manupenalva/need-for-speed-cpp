#ifndef BRIDGES_SHEET_H
#define BRIDGES_SHEET_H

#include <map>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "../../config/config_reader.h"
#include "../sprite.h"

class BridgesSheet {
private:
    SDL2pp::Renderer& renderer;
    std::map<MapType, std::vector<Bridge>> bridges;

    void load_bridges();

public:
    explicit BridgesSheet(SDL2pp::Renderer& renderer);

    std::vector<Sprite> get_bridges(SDL2pp::Texture& map_texture, MapType map_type);
};

#endif
