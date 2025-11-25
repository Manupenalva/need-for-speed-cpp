#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "animations/burst_sheet.h"
#include "animations/fire_sheet.h"
#include "car/car_sheet.h"
#include "countdown/countdown_sheet.h"
#include "map/map_sheet.h"
#include "minimap/minimap_sheet.h"
#include "race/race_sheet.h"
#include "screen/statistics_screen_sheet.h"
#include "screen/upgrade_screen_sheet.h"

#include "sprite.h"

class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    CarSheet car_sheet;
    MapSheet map_sheet;
    RaceSheet race_sheet;
    FireSheet fire_sheet;
    BurstSheet burst_sheet;
    MinimapSheet minimap_sheet;
    CountdownSheet countdown_sheet;
    UpgradeScreenSheet upgrade_screen_sheet;
    StatisticsScreenSheet statistics_screen_sheet;

public:
    explicit TextureManager(SDL2pp::Renderer& renderer);

    void load_resources();

    // Métodos para obtener las texturas de los sprites
    Sprite get_car_sprite(int car_id, int rotation);

    // paso un section_x e y para identificar la sección del mapa (ambos seran la pos actual del
    // auto)
    Sprite get_map_sprite(int map_id, int section_x, int section_y);

    // Metodo para obtener un sprite de carrera (flecha, checkpoint, salida, llegada, etc)
    Sprite_rotation get_race_sprite(int race_element, float direction);

    // Metodo para obtener el sprite de fuego
    Sprite get_fire_sprite(int fire_type);

    // Metodo para obtener el sprite de explosión
    Sprite get_burst_sprite(int burst_type);

    SDL2pp::Texture& get_minimap_texture();

    void load_minimap_info(const MinimapInfo& info, const MapType map_type);

    Sprite get_countdown_sprite(int countdown_type);

    // Metodo para obtener el sprite de la pantalla de mejoras
    Sprite get_upgrade_screen_sprite();

    // Metodo para obtener el sprite de la pantalla de estadísticas
    Sprite get_statistics_screen_sprite();
};

#endif
