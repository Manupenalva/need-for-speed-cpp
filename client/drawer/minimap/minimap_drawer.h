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

#define NORMAL_OPACITY 255
#define BEHIND_MINIMAP_OPACITY 100

class MinimapDrawer: public Drawer {
private:
    MinimapInfo minimap_info;
    bool car_behind;

    MapResources calculate_map_rects(const RenderedState& rendered_state,
                                     const SDL2pp::Texture& tex, Scale scale_factors);
    void is_car_behind(const MapResources& map_info, const RenderedState& rendered_state,
                       SDL2pp::Texture& texture);
    void draw_map(const MapResources& map_info, SDL2pp::Texture& texture);
    void draw_car(const MapResources& map_info, const CarState& client_car, Scale scale_factors);
    void draw_border(const MapResources& map_info);

public:
    explicit MinimapDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
