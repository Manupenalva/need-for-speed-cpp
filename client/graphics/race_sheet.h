#ifndef RACE_SHEET_H
#define RACE_SHEET_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../config/constants.h"

#include "sprite.h"

constexpr const char* ARROW_PATH = "Flecha.png";
constexpr const char* CHECKPOINT_PATH = "Checkpoint.png";
constexpr const char* START_LINE_PATH = "Salida.png";

enum RaceElement {
    RACE_ARROW = 0,
    RACE_CHECKPOINT = 1,
    RACE_START_LINE = 2,
};

class RaceSheet {
private:
    SDL2pp::Renderer& renderer;
    std::vector<Sprite> race_sprites;
    std::string race_path;

public:
    RaceSheet(SDL2pp::Renderer& renderer, const std::string& race_path);

    void load_sprites();

    Sprite_rotation get_race_sprite(const RaceElement RaceType, float direction);
};

#endif
