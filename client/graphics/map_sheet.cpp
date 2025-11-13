#include "map_sheet.h"

MapSheet::MapSheet(SDL2pp::Renderer& renderer): renderer(renderer), map_textures() {}

void MapSheet::load_sprites() {
    // Cargar texturas de mapas
    std::vector<std::pair<MapType, std::string>> map_files = {{LIBERTY_CITY, LIBERTY_CITY_PATH},
                                                              {SAN_ANDREAS, SAN_ANDREAS_PATH},
                                                              {VICE_CITY, VICE_CITY_PATH}};
    for (const auto& [map_type, file_path]: map_files) {
        auto texture = std::make_unique<SDL2pp::Texture>(renderer, SDL2pp::Surface(file_path));
        if (texture->Get() == nullptr) {
            throw std::runtime_error("Error al cargar textura de mapa: " + file_path);
        }

        map_textures.emplace(map_type, std::move(texture));
    }
}

Sprite MapSheet::get_map_sprite(MapType map_type, int section_x, int section_y) {
    auto it = map_textures.find(map_type);
    if (it == map_textures.end()) {
        throw std::runtime_error("Map type not found in textures.");
    }

    SDL2pp::Texture& texture = *(it->second);
    SDL2pp::Rect src_rect = get_map_section_rect(texture, section_x, section_y);

    return Sprite{texture, src_rect};
}

SDL2pp::Rect MapSheet::get_map_section_rect(SDL2pp::Texture& texture, int section_x,
                                            int section_y) {

    ConfigReader& config = ConfigReader::get_instance();
    const int view_width = config.get_window_width();    // Ancho de la vista del juego
    const int view_height = config.get_window_height();  // Alto de la vista del juego
    const int map_width = texture.GetWidth();
    const int map_height = texture.GetHeight();

    int left = section_x - HALF(view_width);
    int top = section_y - HALF(view_height);

    // Asegurarse de que el rectángulo no salga de los límites del mapa
    if (left < 0)
        left = MAP_MIN_X;
    if (top < 0)
        top = MAP_MIN_Y;
    if (left + view_width > map_width)
        left = map_width - view_width;
    if (top + view_height > map_height)
        top = map_height - view_height;

    return SDL2pp::Rect(left, top, view_width, view_height);
}
