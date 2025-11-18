#include "minimap_sheet.h"

#include <stdexcept>

MinimapSheet::MinimapSheet(SDL2pp::Renderer& renderer, RaceSheet& race_sheet):
        renderer(renderer), texture(), race_sheet(race_sheet) {}

void MinimapSheet::load_texture(const MapType map_type) {
    // Cargar texturas de minimapas
    std::string file_path = MAP_PATHS.at(map_type);
    SDL2pp::Texture tex(renderer, SDL2pp::Surface(file_path));
    if (tex.Get() == nullptr) {
        throw std::runtime_error("Error al cargar textura de minimapa: " + file_path);
    }

    auto renderable = std::make_unique<SDL2pp::Texture>(renderer, SDL_PIXELFORMAT_RGBA8888,
                                                        SDL_TEXTUREACCESS_TARGET, tex.GetWidth(),
                                                        tex.GetHeight());

    renderer.SetTarget(*renderable);
    renderer.Copy(tex);
    renderer.SetTarget();

    texture = std::move(renderable);
}

void MinimapSheet::load_minimap_info(const MinimapInfo& info) {
    renderer.SetTarget(*texture);

    // Dibujar flechas
    for (const auto& arrow: info.arrows) {
        Sprite_rotation arrow_sprite = race_sheet.get_race_sprite(RACE_ARROW, arrow.angle);

        SDL2pp::Rect dst(arrow.x - HALF(arrow_sprite.sprite.src_rect.w),
                         arrow.y - HALF(arrow_sprite.sprite.src_rect.h),
                         arrow_sprite.sprite.src_rect.w * 2, arrow_sprite.sprite.src_rect.h * 2);

        renderer.Copy(arrow_sprite.sprite.texture, arrow_sprite.sprite.src_rect, dst,
                      arrow_sprite.rotation);
    }

    // Dibujar checkpoints
    for (const auto& checkpoint: info.checkpoints) {
        Sprite_rotation cp_sprite = race_sheet.get_race_sprite(
                static_cast<RaceElement>(checkpoint.type), checkpoint.angle);

        SDL2pp::Rect dst(checkpoint.x - HALF(cp_sprite.sprite.src_rect.w),
                         checkpoint.y - HALF(cp_sprite.sprite.src_rect.h),
                         cp_sprite.sprite.src_rect.w * 2, cp_sprite.sprite.src_rect.h * 2);

        renderer.Copy(cp_sprite.sprite.texture, cp_sprite.sprite.src_rect, dst, cp_sprite.rotation);
    }

    renderer.SetTarget();
}

SDL2pp::Texture& MinimapSheet::get_minimap_texture() {
    if (!texture) {
        throw std::runtime_error("Minimap texture not loaded.");
    }
    return *texture;
}
