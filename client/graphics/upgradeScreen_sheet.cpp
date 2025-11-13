#include "upgradeScreen_sheet.h"

UpgradeScreenSheet::UpgradeScreenSheet(SDL2pp::Renderer& renderer): renderer(renderer), texture() {}


void UpgradeScreenSheet::load_sprite() {
    SDL2pp::Surface surface(UPGRADE_SCREEN_PATH);
    texture = std::make_unique<SDL2pp::Texture>(renderer, surface);
    if (texture->Get() == nullptr) {
        throw std::runtime_error("Error al cargar textura de pantalla de mejoras: " +
                                 std::string(UPGRADE_SCREEN_PATH));
    }
}

Sprite UpgradeScreenSheet::get_screen_sprite() {
    SDL2pp::Rect src_rect(MAP_MIN_X, MAP_MIN_Y, texture->GetWidth(), texture->GetHeight());
    return Sprite{*texture, src_rect};
}
