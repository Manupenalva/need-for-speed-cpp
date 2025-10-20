#ifndef MAP_SHEET_H
#define MAP_SHEET_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include "sprite.h"
#include <unordered_map>
#include <vector>

#define HALF(x) ((x) / 2)
#define MAP_MIN_X 0
#define MAP_MIN_Y 0

enum MapType {
    LIBERTY_CITY = 1,
    SAN_ANDREAS = 2,
    VICE_CITY = 3,
};

class MapSheet {
    private:
    // Cargara los mapas completos y get_map_sprite se encargara de devolver la sección correcta que nos interesa en base a la posicion del auto.
    std::unordered_map<MapType, SDL2pp::Texture> map_sprites;

    SDL2pp::Rect get_map_section_rect(SDL2pp::Texture& texture, int section_x, int section_y);

public:
    MapSheet(SDL2pp::Renderer& renderer, const std::string& map_path);

    void load_sprites(const std::string& map_path, SDL2pp::Renderer& renderer);

    sprite get_map_sprite(MapType map_type, int section_x, int section_y);
};

#endif