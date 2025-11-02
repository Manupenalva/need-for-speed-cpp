#include "race_sheet.h"

RaceSheet::RaceSheet(SDL2pp::Renderer& renderer, const std::string& race_path):
        renderer(renderer), race_sprites() {}

void RaceSheet::load_sprites() {
    // Cargar texturas de carrera
    std::vector<std::pair<RaceElement, std::string>> race_files = {
            {RACE_ARROW, ARROW_PATH},
            {RACE_CHECKPOINT, CHECKPOINT_PATH},
            {RACE_START_LINE, START_LINE_PATH}};
    for (const auto& [race_element, file_path]: race_files) {
        SDL2pp::Texture texture(renderer, SDL2pp::Surface(file_path));
        if (texture.Get() == nullptr) {
            throw std::runtime_error("Error al cargar textura de carrera: " + file_path);
        }

        SDL2pp::Rect src_rect(MAP_MIN_X, MAP_MIN_Y, texture.GetWidth(), texture.GetHeight());
        race_sprites.emplace_back(texture, src_rect);
    }
}

sprite RaceSheet::get_race_sprite(const RaceElement RaceType, float direction) {
    int index = static_cast<int>(RaceType);
    return race_sprites[index];
}
