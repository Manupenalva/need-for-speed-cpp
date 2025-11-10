#include "upgradeScreen_sheet.h"

UpgradeScreenSheet::UpgradeScreenSheet(SDL2pp::Renderer& renderer,
                                       const std::string& upgrade_screen_path):
        renderer(renderer), texture(), upgrade_screen_path(upgrade_screen_path) {}


void UpgradeScreenSheet::load_sprite() {
    SDL2pp::Surface surface(upgrade_screen_path);
    texture = std::make_unique<SDL2pp::Texture>(renderer, surface);
    if (texture->Get() == nullptr) {
        throw std::runtime_error("Error al cargar textura de pantalla de mejoras: " +
                                 upgrade_screen_path);
    }
}

Sprite UpgradeScreenSheet::get_screen_sprite() {
    SDL2pp::Rect src_rect(MAP_MIN_X, MAP_MIN_Y, texture->GetWidth(), texture->GetHeight());
    return Sprite{*texture, src_rect};
}
