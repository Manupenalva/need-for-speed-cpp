#ifndef HEALTH_DRAWER_H
#define HEALTH_DRAWER_H

#include "../../config/config_reader.h"
#include "../drawer.h"

#define BAR_W 200
#define BAR_H 20
#define BAR_X 30
#define BAR_Y_OFFSET 30

struct Bar_dimensions {
    int w;
    int h;
    int x;
    int y;
};

struct Bar_colors {
    int green_w;
    int red_w;
};

class HealthDrawer: public Drawer {
private:
    Bar_dimensions calcular_bar_dimensions(float scale_x, float scale_y, int win_h);
    Bar_colors calcular_bar_colors(int total_w, const CarState& client_car);
    void draw_bar(const Bar_dimensions& dims, const Bar_colors& colors, float scale_x,
                  float scale_y);

public:
    explicit HealthDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager);

    void draw(RenderedState& rendered_state) override;
};

#endif
