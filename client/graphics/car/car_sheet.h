#ifndef CAR_SHEET_H
#define CAR_SHEET_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../config/constants.h"
#include "../sprite.h"

#define ROTATIONS 16

#define ROTATIONS_PER_ROW 8
#define ROWS_PER_CAR 2

#define CAR_DIMENSION_1 \
    { 32, 32 }
#define CAR_DIMENSION_2 \
    { 40, 40 }
#define CAR_DIMENSION_3 \
    { 48, 48 }

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
    std::unordered_map<CarType, std::vector<Sprite>> car_sprites;

    int get_rotation_index(float rotation);

public:
    explicit CarSheet(SDL2pp::Renderer& renderer);

    void load_sprites();

    Sprite get_car_sprite(CarType car_type, float rotation);
};

#endif
