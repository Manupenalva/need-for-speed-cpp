#include "car_sheet.h"

CarSheet::CarSheet(SDL2pp::Renderer& renderer, const std::string& car_path):
        texture(renderer, [&car_path]() {
            SDL2pp::Surface surface(car_path);

            // Obtener color del píxel (0,0)
            uint32_t* pixel_color = static_cast<uint32_t*>(surface.Get()->pixels);
            uint32_t color_key = pixel_color[0];
            surface.SetColorKey(true, color_key);

            // Retornar la superficie modificada
            return surface;
        }()) {
    if (texture.Get() == nullptr) {
        throw std::runtime_error("Error al cargar textura de autos: " + car_path);
    }
}

void CarSheet::load_sprites() {
    // Aquí se cargarían los sprites específicos de cada tipo de auto
    // Por simplicidad, asumimos que cada auto tiene 8 rotaciones y están
    // organizados en una cuadrícula en la imagen cargada.

    const std::unordered_map<CarType, std::pair<int, int>> car_dimensions = {
            {CAR_TYPE_1, {32, 32}}, {CAR_TYPE_2, {40, 40}}, {CAR_TYPE_3, {40, 40}},
            {CAR_TYPE_4, {40, 40}}, {CAR_TYPE_5, {40, 40}}, {CAR_TYPE_6, {40, 40}},
            {CAR_TYPE_7, {48, 48}}};

    for (int car_type = CAR_TYPE_1; car_type <= CAR_TYPE_7; ++car_type) {
        std::vector<sprite> sprites_for_type;
        for (int rotation = 0; rotation < ROTATIONS; ++rotation) {
            SDL2pp::Rect src_rect(
                    rotation * car_dimensions.at(static_cast<CarType>(car_type)).first,
                    (car_type - 1) * car_dimensions.at(static_cast<CarType>(car_type)).second,
                    car_dimensions.at(static_cast<CarType>(car_type)).first,
                    car_dimensions.at(static_cast<CarType>(car_type)).second);
            sprites_for_type.emplace_back(sprite{texture, src_rect});
        }
        car_sprites.emplace(static_cast<CarType>(car_type), std::move(sprites_for_type));
    }
}


sprite CarSheet::get_car_sprite(CarType car_type, float rotation) {
    int rotation_index = get_rotation_index(rotation);

    auto it = car_sprites.find(car_type);
    if (it != car_sprites.end()) {
        const std::vector<sprite>& sprites = it->second;
        return sprites[rotation_index];
    }
    throw std::runtime_error("Car type or rotation not found in sprites.");
}

int CarSheet::get_rotation_index(float rotation) {
    // Convertir radianes a grados
    float degrees = rotation * (180.0f / M_PI);
    // Asegurarse de que el ángulo esté en el rango [0, 360)

    if (degrees < 0) {
        degrees += 360.0f;
    }
    // Calcular el índice de rotación basado en 16 rotaciones
    int index = static_cast<int>((degrees / 360.0f) * ROTATIONS) % ROTATIONS;
    return index;
}
