#include "texture_manager.h"

TextureManager::TextureManager(SDL2pp::Renderer& renderer, const std::string& assets_path):
        renderer(renderer),
        car_sheet(renderer, assets_path + CAR_PATH),
        map_sheet(renderer, assets_path + MAP_PATH),
        race_sheet(renderer, assets_path + RACE_PATH),
        upgrade_screen_sheet(renderer, assets_path + UPGRADE_SCREEN_PATH),
        fire_sheet(renderer),
        crash_sheet(renderer) {}


void TextureManager::load_resources() {
    car_sheet.load_sprites();
    map_sheet.load_sprites();
    race_sheet.load_sprites();
    upgrade_screen_sheet.load_sprite();
    fire_sheet.load_sprites();
    crash_sheet.load_sprites();
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

Sprite TextureManager::get_crash_sprite(int crash_type) {
    return crash_sheet.get_sprite(crash_type);
}
