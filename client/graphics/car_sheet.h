#ifndef CAR_SHEET_H
#define CAR_SHEET_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include "sprite.h"
#include <unordered_map>
#include <vector>

#define ROTATIONS 16

enum CarType {
    CAR_TYPE_1 = 1,
    CAR_TYPE_2 = 2,
    CAR_TYPE_3 = 3,
    CAR_TYPE_4 = 4,
    CAR_TYPE_5 = 5,
    CAR_TYPE_6 = 6,
    CAR_TYPE_7 = 7
};
 

class CarSheet {
private:
    SDL2pp::Texture texture;
    std::unordered_map<CarType, std::vector<sprite>> car_sprites;

    int get_rotation_index(float rotation);

public:
    CarSheet(SDL2pp::Renderer& renderer, const std::string& car_path);

    void load_sprites(SDL2pp::Renderer& renderer);

    sprite get_car_sprite(CarType car_type, float rotation);
};

#endif
