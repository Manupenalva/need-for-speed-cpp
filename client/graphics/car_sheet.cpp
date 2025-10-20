#include "car_sheet.h"

#include <SDL2pp/SDL_image.h>

CarSheet::CarSheet(SDL2pp::Renderer& renderer, const std::string& car_path)
    : texture(renderer, SDL2pp::Surface(IMG_Load(car_path.c_str()))) {
    if (texture.Get() == nullptr) {
        throw std::runtime_error("Error al cargar textura de autos: " + car_path);
    }
}

void CarSheet::load_sprites(SDL2pp::Renderer& renderer) {
    // Aquí se cargarían los sprites específicos de cada tipo de auto
    // Por simplicidad, asumimos que cada auto tiene 8 rotaciones y están
    // organizados en una cuadrícula en la imagen cargada.
    
    const std::unordered_map<CarType, std::pair<int, int>> car_dimensions = {
        {CAR_TYPE_1, {32, 32}},
        {CAR_TYPE_2, {40, 40}},
        {CAR_TYPE_3, {40, 40}},
        {CAR_TYPE_4, {40, 40}},
        {CAR_TYPE_5, {40, 40}},
        {CAR_TYPE_6, {40, 40}},
        {CAR_TYPE_7, {48, 48}}
    };

    for (int car_type = CAR_TYPE_1; car_type <= CAR_TYPE_7; ++car_type) {
        std::vector<sprite> sprites_for_type;
        for (int rotation = 0; rotation < ROTATIONS; ++rotation) {
            SDL2pp::Rect src_rect(rotation * car_dimensions.at(static_cast<CarType>(car_type)).first,
                                  (car_type - 1) * car_dimensions.at(static_cast<CarType>(car_type)).second,
                                  car_dimensions.at(static_cast<CarType>(car_type)).first,
                                  car_dimensions.at(static_cast<CarType>(car_type)).second);
            sprites_for_type.emplace_back(sprite{texture, src_rect});
        }
        car_sprites.emplace(static_cast<CarType>(car_type), std::move(sprites_for_type));
    }
}


sprite CarSheet::get_car_sprite(CarType car_type, int rotation) {
    rotation = rotation % ROTATIONS;
    if (rotation < 0) {
        rotation += ROTATIONS;
    }

    auto it = car_sprites.find(car_type);
    if (it != car_sprites.end()) {
        const std::vector<sprite>& sprites = it->second;
        if (rotation < sprites.size()) {
            return sprites[rotation];
        }
    }
    throw std::runtime_error("Car type or rotation not found in sprites.");
}
