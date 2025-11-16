#include "base_sheet.h"

AnimationSheet::AnimationSheet(SDL2pp::Renderer& renderer, const std::string& path):
        texture(renderer, [&path]() {
            SDL2pp::Surface surface(path);

            uint32_t* pixel_color = static_cast<uint32_t*>(surface.Get()->pixels);
            uint32_t color_key = pixel_color[0];
            surface.SetColorKey(true, color_key);

            return surface;
        }()) {
    if (texture.Get() == nullptr) {
        throw std::runtime_error("Error al cargar textura de animación: " + path);
    }
}

void AnimationSheet::load_sprites() {
    int w = width_image / types_per_row;
    int h = height_image / types_per_col;

    for (int animation_type = 0; animation_type < types_per_row * types_per_col; ++animation_type) {
        int col = animation_type % types_per_row;
        int row = animation_type / types_per_row;

        SDL2pp::Rect src_rect(col * w, row * h, w, h);

        animation_sprites.emplace(animation_type, Sprite{texture, src_rect});
    }
}

Sprite AnimationSheet::get_sprite(int animation_type) {
    auto it = animation_sprites.find(animation_type);
    if (it != animation_sprites.end()) {
        return it->second;
    }
    throw std::runtime_error("Animation type not found in sprites.");
}
