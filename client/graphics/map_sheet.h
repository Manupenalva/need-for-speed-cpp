#ifndef MAP_SHEET_H
#define MAP_SHEET_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../config/config_reader.h"
#include "../config/constants.h"

#include "sprite.h"

enum MapType {
    LIBERTY_CITY = 1,
    SAN_ANDREAS = 2,
    VICE_CITY = 3,
};

class MapSheet {
private:
    // Cargara los mapas completos y get_map_sprite se encargara de devolver la sección correcta que
    // nos interesa en base a la posicion del auto.
    SDL2pp::Renderer& renderer;
    std::unordered_map<MapType, std::unique_ptr<SDL2pp::Texture>> map_textures;

    SDL2pp::Rect get_map_section_rect(SDL2pp::Texture& texture, int section_x, int section_y);

public:
    explicit MapSheet(SDL2pp::Renderer& renderer);

    void load_sprites();

    Sprite get_map_sprite(MapType map_type, int section_x, int section_y);
};

#endif
