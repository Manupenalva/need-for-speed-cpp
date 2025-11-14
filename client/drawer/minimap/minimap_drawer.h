#ifndef MINIMAP_DRAWER_H
#define MINIMAP_DRAWER_H

#include "../../config/config_reader.h"
#include "../../config/constants.h"
#include "../drawer.h"

#include "mapInfoDTO.h"

#define MAP_BORDER_COLOR_R 0
#define MAP_BORDER_COLOR_G 0
#define MAP_BORDER_COLOR_B 0
#define MAP_BORDER_COLOR_A 255

#define BORDER_THICKNESS 2
#define RESOURCE_DIVISOR 4

class MinimapDrawer: public Drawer {
private:
    MapResources calculate_map_rects(const RenderedState& rendered_state,
                                     const SDL2pp::Texture& tex);
    void draw_map(const MapResources& map_info, SDL2pp::Texture& texture);
    void draw_car(const MapResources& map_info, const CarState& client_car);
    void draw_border(const MapResources& map_info);
    void draw_arrows(const MapResources& map_info, const MinimapInfo& minimap_info);
    void draw_checkpoints(const MapResources& map_info, const MinimapInfo& minimap_info);

public:
    explicit MinimapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(const RenderedState& rendered_state) override;
};

#endif
