#include "race_sheet.h"

RaceSheet::RaceSheet(SDL2pp::Renderer& renderer, const std::string& race_path):
        renderer(renderer), race_sprites(), race_path(race_path) {}

void RaceSheet::load_sprites() {
    // Cargar texturas de carrera
    std::vector<std::pair<RaceElement, std::string>> race_files = {
            {RACE_ARROW, race_path + ARROW_PATH},
            {RACE_CHECKPOINT, race_path + CHECKPOINT_PATH},
            {RACE_START_LINE, race_path + START_LINE_PATH}};
    for (const auto& [race_element, file_path]: race_files) {
        SDL2pp::Surface surface(file_path);

        const uint32_t* pixel_color = static_cast<uint32_t*>(surface.Get()->pixels);
        uint32_t color_key = pixel_color[0];
        surface.SetColorKey(true, color_key);

        auto texture = std::make_unique<SDL2pp::Texture>(renderer, surface);
        if (texture->Get() == nullptr) {
            throw std::runtime_error("Error al cargar textura de carrera: " + file_path);
        }

        SDL2pp::Rect src_rect(MAP_MIN_X, MAP_MIN_Y, texture->GetWidth(), texture->GetHeight());

        race_sprites.emplace_back(Sprite{*texture, src_rect});
        textures.push_back(std::move(texture));
    }
}

Sprite_rotation RaceSheet::get_race_sprite(const RaceElement RaceType, float direction) {
    int index = static_cast<int>(RaceType);
    const Sprite& sprite = race_sprites[index];
    return Sprite_rotation{sprite, direction};
}
