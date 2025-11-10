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

Car CarBuilder::create_car(const int& id, const int& car_type) {
    try {
        if (!cars_data) {
            throw std::runtime_error("Invalid YAML archive");
        }

        YAML::Node car_parameters = cars_data[car_type];

        std::string name = car_parameters["name"].as<std::string>();
        float max_speed = car_parameters["max_speed"].as<float>();
        float acceleration = car_parameters["acceleration"].as<float>();
        float health = car_parameters["health"].as<float>();
        float mass = car_parameters["mass"].as<float>();
        float drivability = car_parameters["drivability"].as<float>();
        float car_long = car_parameters["long"].as<float>();
        float car_width = car_parameters["width"].as<float>();

        return Car(id, name, max_speed, acceleration, health, mass, drivability, car_long,
                   car_width, car_type);
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
        for (const auto& car_node: cars_data) {
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
