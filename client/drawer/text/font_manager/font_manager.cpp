#include "font_manager.h"

FontManager::FontManager() {}

SDL2pp::Font& FontManager::get_font(const FontKey& key) {
    auto it = fonts.find(key);
    if (it != fonts.end()) {
        return *(it->second);
    } else {
        // Si el tamaño no existe, crear y almacenar una nueva fuente
        auto font = std::make_unique<SDL2pp::Font>(key.type, key.size);
        SDL2pp::Font& font_ref = *font;
        fonts[key] = std::move(font);
        return font_ref;
    }
}
