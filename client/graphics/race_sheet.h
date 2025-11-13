#ifndef RACE_SHEET_H
#define RACE_SHEET_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../config/constants.h"

#include "sprite.h"


enum RaceElement {
    RACE_ARROW = 0,
    RACE_CHECKPOINT = 1,
    RACE_START_LINE = 2,
};

class RaceSheet {
private:
    SDL2pp::Renderer& renderer;
    std::vector<std::unique_ptr<SDL2pp::Texture>> textures;
    std::vector<Sprite> race_sprites;

public:
    explicit RaceSheet(SDL2pp::Renderer& renderer);

    void load_sprites();

    Sprite_rotation get_race_sprite(const RaceElement RaceType, float direction);
};

#endif
