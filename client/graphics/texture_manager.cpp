#include "texture_manager.h"

TextureManager::TextureManager(SDL2pp::Renderer& renderer):
        renderer(renderer),
        car_sheet(renderer),
        map_sheet(renderer),
        race_sheet(renderer),
        upgrade_screen_sheet(renderer),
        fire_sheet(renderer),
        burst_sheet(renderer) {}


void TextureManager::load_resources() {
    car_sheet.load_sprites();
    map_sheet.load_sprites();
    race_sheet.load_sprites();
    upgrade_screen_sheet.load_sprite();
    fire_sheet.load_sprites();
    burst_sheet.load_sprites();
}

Sprite TextureManager::get_car_sprite(int car_id, int rotation) {
    CarType car_type = static_cast<CarType>(car_id);
    return car_sheet.get_car_sprite(car_type, rotation);
}

Sprite TextureManager::get_map_sprite(int map_id, int section_x, int section_y) {
    MapType map_type = static_cast<MapType>(map_id);
    return map_sheet.get_map_sprite(map_type, section_x, section_y);
}

Sprite_rotation TextureManager::get_race_sprite(int race_element, float direction) {
    RaceElement race_type = static_cast<RaceElement>(race_element);
    return race_sheet.get_race_sprite(race_type, direction);
}

Sprite TextureManager::get_upgrade_screen_sprite() {
    return upgrade_screen_sheet.get_screen_sprite();
}

Sprite TextureManager::get_fire_sprite(int fire_type) { return fire_sheet.get_sprite(fire_type); }

Sprite TextureManager::get_burst_sprite(int burst_type) {
    return burst_sheet.get_sprite(burst_type);
}
