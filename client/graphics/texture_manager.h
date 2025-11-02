#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "car_sheet.h"
#include "map_sheet.h"
#include "race_sheet.h"
#include "sprite.h"

#define CAR_PATH "cars/Cars.png"
#define PEOPLE_PATH "people/Mobile - Grand Theft Auto 4 - Miscellaneous - People.png"
#define MAP_PATH "cities/"
#define RACE_PATH "race/"

class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    CarSheet car_sheet;
    MapSheet map_sheet;
    RaceSheet race_sheet;

public:
    TextureManager(SDL2pp::Renderer& renderer, const std::string& assets_path);

    void load_resources();

    // Métodos para obtener las texturas de los sprites
    sprite get_car_sprite(int car_id, int rotation);

    // paso un section_x e y para identificar la sección del mapa (ambos seran la pos actual del
    // auto)
    sprite get_map_sprite(int map_id, int section_x, int section_y);

    // Metodo para obtener un sprite de carrera (flecha, checkpoint, salida, llegada, etc)
    sprite get_race_sprite(int race_element, float direction);
};

#endif
