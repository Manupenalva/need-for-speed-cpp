#include "carBuilder.h"

#include <iostream>

CarBuilder::CarBuilder(const std::string& path) {
    try {
        cars_data = YAML::LoadFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading cars_config file: " << e.what() << std::endl;
        throw;
    }
}

CarBuilder::CarBuilder(const std::string& path, std::shared_ptr<CarConstants> constants)
    : car_constants(constants) {
    try {
        cars_data = YAML::LoadFile(path);
        load_constants();
    } catch (const std::exception& e) {
        std::cerr << "Error loading cars_config file: " << e.what() << std::endl;
        throw;
    }
}

Car CarBuilder::create_car(const int& id, const int& car_type) {
    try {
        if (!cars_data) {
            throw std::runtime_error("Invalid YAML archive");
        }
        if (!car_constants) {
            throw std::runtime_error("Car constants not loaded, use the appropriate constructor");
        }
        YAML::Node cars_specs = cars_data["cars"];
        int selected_car_type = car_type;
        if (car_type <= 0 || car_type > static_cast<int>(cars_specs.size())) {
            selected_car_type = 1 + (std::rand() % (cars_specs.size()));
        }

        YAML::Node car_parameters = cars_specs[selected_car_type];

        std::string name = car_parameters["name"].as<std::string>();
        float max_speed = car_parameters["max_speed"].as<float>();
        float acceleration = car_parameters["acceleration"].as<float>();
        float health = car_parameters["health"].as<float>();
        float mass = car_parameters["mass"].as<float>();
        float drivability = car_parameters["drivability"].as<float>();
        float car_long = car_parameters["long"].as<float>();
        float car_width = car_parameters["width"].as<float>();

        return Car(id, name, max_speed, acceleration, health, mass, drivability, car_long,
                   car_width, selected_car_type, car_constants);
    } catch (const std::exception& e) {
        std::cerr << "Error building the car: " << e.what() << std::endl;
        throw;
    }
}

std::vector<CarProperties> CarBuilder::get_catalog() {
    try {
        if (!cars_data) {
            throw std::runtime_error("Invalid YAML archive");
        }

        std::vector<CarProperties> catalog;
        uint8_t car_id = 1;
        for (const auto& car_node: cars_data["cars"]) {
            const auto& car_parameters = car_node.second;

            CarProperties car_properties;
            car_properties.car_id = car_id;
            car_properties.max_speed = car_parameters["max_speed"].as<uint16_t>();
            car_properties.acceleration = car_parameters["acceleration"].as<uint16_t>();
            car_properties.max_health = car_parameters["health"].as<uint16_t>();
            car_properties.mass = car_parameters["mass"].as<uint16_t>();
            car_properties.control = car_parameters["drivability"].as<uint16_t>();
            car_id++;
            catalog.push_back(car_properties);
        }

        return catalog;
    } catch (const std::exception& e) {
        std::cerr << "Error getting the catalog: " << e.what() << std::endl;
        throw;
    }
}

void CarBuilder::load_constants() {
    try {
        if (!cars_data) {
            throw std::runtime_error("Invalid YAML archive");
        }
        YAML::Node constants_node = cars_data["constants"];
        YAML::Node car_node = constants_node["car_constants"];
        YAML::Node physics_node = constants_node["physical_constants"];

        car_constants->car.MAX_SPEED = car_node["MAX_SPEED"].as<float>();
        car_constants->car.MAX_ACCELERATION = car_node["MAX_ACCELERATION"].as<float>();
        car_constants->car.MAX_HEALTH = car_node["MAX_HEALTH"].as<float>();
        car_constants->car.MAX_MASS = car_node["MAX_MASS"].as<float>();
        car_constants->car.MAX_DRIVABILITY = car_node["MAX_DRIVABILITY"].as<float>();

        car_constants->physics.BASE_MAX_SPEED = physics_node["BASE_MAX_SPEED"].as<float>();
        car_constants->physics.BASE_ACCELERATION = physics_node["BASE_ACCELERATION"].as<float>();
        car_constants->physics.BASE_ANGLE_ROTATION = physics_node["BASE_ANGLE_ROTATION"].as<float>();
        car_constants->physics.BASE_FRICTION = physics_node["BASE_FRICTION"].as<float>();
        car_constants->physics.MIN_SPEED = physics_node["MIN_SPEED"].as<float>();
        car_constants->physics.LIGHT_CRASH_DAMAGE = physics_node["LIGHT_CRASH_DAMAGE"].as<float>();
        car_constants->physics.MEDIUM_CRASH_DAMAGE = physics_node["MEDIUM_CRASH_DAMAGE"].as<float>();
        car_constants->physics.HEAVY_CRASH_DAMAGE = physics_node["HEAVY_CRASH_DAMAGE"].as<float>();
        car_constants->physics.MEDIUM_CRASH_LIMIT = physics_node["MEDIUM_CRASH_LIMIT"].as<float>();
        car_constants->physics.HEAVY_CRASH_LIMIT = physics_node["HEAVY_CRASH_LIMIT"].as<float>();
        car_constants->physics.REVERSE_ACCELERATION_FACTOR = physics_node["REVERSE_ACCELERATION_FACTOR"].as<float>();
        car_constants->physics.REVERSE_SPEED_FACTOR = physics_node["REVERSE_SPEED_FACTOR"].as<float>();
        car_constants->physics.MIN_GAME_SPEED = physics_node["MIN_GAME_SPEED"].as<float>();
        car_constants->physics.NITRO_USE = physics_node["NITRO_USE"].as<float>();
        car_constants->physics.NITRO_ACCELERATION_FACTOR = physics_node["NITRO_ACCELERATION_FACTOR"].as<float>();
    } catch (const std::exception& e) {
        std::cerr << "Error loading constants: " << e.what() << std::endl;
        throw;
    }
}
