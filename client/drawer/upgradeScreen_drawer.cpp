#include "upgradeScreen_drawer.h"

UpgradeScreenDrawer::UpgradeScreenDrawer(SDL2pp::Renderer& renderer,
                                         TextureManager& texture_manager):
        renderer(renderer), texture_manager(texture_manager) {}

void UpgradeScreenDrawer::draw() {
    Sprite upgrade_screen_sprite = texture_manager.get_upgrade_screen_sprite();

    ConfigReader& config = ConfigReader::get_instance();

    SDL2pp::Rect dst_rect(MAP_MIN_X, MAP_MIN_Y, config.get_window_width(),
                          config.get_window_height());

    renderer.Copy(upgrade_screen_sprite.texture, upgrade_screen_sprite.src_rect, dst_rect);
}
