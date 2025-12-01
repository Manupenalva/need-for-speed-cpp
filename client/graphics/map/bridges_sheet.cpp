#include "bridges_sheet.h"

BridgesSheet::BridgesSheet(SDL2pp::Renderer& renderer): renderer(renderer), bridges() {
    load_bridges();
}

void BridgesSheet::load_bridges() {
    ConfigReader& config = ConfigReader::get_instance();
    bridges = config.get_bridges_for_map();
}

std::vector<Sprite> BridgesSheet::get_bridges(SDL2pp::Texture& map_texture, MapType map_type) {
    std::vector<Sprite> sprites;
    for (const auto& bridge: bridges[map_type]) {
        SDL2pp::Rect src_rect(static_cast<int>(bridge.x), static_cast<int>(bridge.y),
                              static_cast<int>(bridge.width), static_cast<int>(bridge.height));
        sprites.push_back(Sprite{map_texture, src_rect});
    }

    return sprites;
}
