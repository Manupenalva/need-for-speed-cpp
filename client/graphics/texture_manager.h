#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include "car_sheet.h"
#include "people_sheet.h"
#include "map_sheet.h"
#include "sprite.h"

#define CAR_PATH "cars/Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png"
#define PEOPLE_PATH "people/Mobile - Grand Theft Auto 4 - Miscellaneous - People.png"
#define MAP_PATH "map/"

class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    CarSheet car_sheet;
    //PeopleSheet people_sheet;
    MapSheet map_sheet;
    
public:
    TextureManager(SDL2pp::Renderer& renderer, const std::string& assets_path);

    void load_resources();

    // Métodos para obtener las texturas de los sprites
    sprite get_car_sprite(int car_id, int rotation);
    
    // sprite get_people_sprite(int people_id, int rotation, int walk_frame);

    // paso un section_x e y para identificar la sección del mapa (ambos seran la pos actual del auto)
    sprite get_map_sprite(int map_id, int section_x, int section_y);

}; 

#endif