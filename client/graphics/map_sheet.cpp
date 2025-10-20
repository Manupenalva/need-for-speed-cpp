#include "map_sheet.h"

#include <SDL2pp/SDL_image.h>

MapSheet::MapSheet(SDL2pp::Renderer& renderer, const std::string& map_path)
    : map_sprites() {}

void MapSheet::load_sprites(const std::string& map_path, SDL2pp::Renderer& renderer) {
    // Cargar texturas de mapas
    std::vector<std::pair<MapType, std::string>> map_files = {
        {LIBERTY_CITY, map_path + "Backgrounds - Liberty City.png"},
        {SAN_ANDREAS, map_path + "Backgrounds - San Andreas.png"},
        {VICE_CITY, map_path + "Backgrounds - Vice City.png"}
    };
    for (const auto& [map_type, file_path] : map_files) {
        SDL2pp::Texture texture(renderer, SDL2pp::Surface(IMG_Load(file_path.c_str())));
        if (texture.Get() == nullptr) {
            throw std::runtime_error("Error al cargar textura de mapa: " + file_path);
        }

        map_sprites.emplace(map_type, texture);
    }
}

sprite MapSheet::get_map_sprite(MapType map_type, int section_x, int section_y) {
    auto it = map_sprites.find(map_type);
    if (it == map_sprites.end()) {
        throw std::runtime_error("Map type not found in sprites.");
    }

    SDL2pp::Texture& texture = it->second;
    SDL2pp::Rect src_rect = get_map_section_rect(texture, section_x, section_y);
    
    return sprite{texture, src_rect};
}

SDL2pp::Rect MapSheet::get_map_section_rect(SDL2pp::Texture& texture, int section_x, int section_y) {
    const int view_width = 800;  // Ancho de la vista del juego
    const int view_height = 600; // Alto de la vista del juego
    const int map_width = texture.GetWidth();
    const int map_height = texture.GetHeight();

    int left = section_x - HALF(view_width);
    int top = section_y - HALF(view_height);

    // Asegurarse de que el rectángulo no salga de los límites del mapa
    if (left < 0) left = MAP_MIN_X;
    if (top < 0) top = MAP_MIN_Y;
    if (left + view_width > map_width) left = map_width - view_width;
    if (top + view_height > map_height) top = map_height - view_height;

    return SDL2pp::Rect(left, top, view_width, view_height);
}