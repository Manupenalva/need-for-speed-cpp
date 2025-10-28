#include "texture_manager.h"

TextureManager::TextureManager(SDL2pp::Renderer& renderer, const std::string& assets_path):
        renderer(renderer),
        car_sheet(renderer, assets_path + CAR_PATH),
        map_sheet(assets_path + MAP_PATH) {}


void TextureManager::load_resources() {
    car_sheet.load_sprites();
    map_sheet.load_sprites(renderer);
}

sprite TextureManager::get_car_sprite(int car_id, int rotation) {
    CarType car_type = static_cast<CarType>(car_id);
    return car_sheet.get_car_sprite(car_type, rotation);
}

sprite TextureManager::get_map_sprite(int map_id, int section_x, int section_y) {
    MapType map_type = static_cast<MapType>(map_id);
    return map_sheet.get_map_sprite(map_type, section_x, section_y);
}
