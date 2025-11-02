#include "texture_manager.h"

TextureManager::TextureManager(SDL2pp::Renderer& renderer, const std::string& assets_path):
        renderer(renderer),
        car_sheet(renderer, assets_path + CAR_PATH),
        map_sheet(renderer, assets_path + MAP_PATH),
        race_sheet(renderer, assets_path + RACE_PATH) {}


void TextureManager::load_resources() {
    car_sheet.load_sprites();
    map_sheet.load_sprites();
    race_sheet.load_sprites();
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
