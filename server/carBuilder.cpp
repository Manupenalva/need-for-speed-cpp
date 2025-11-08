#include "carBuilder.h"

#include <iostream>

YAML::Node CarBuilder::open_file(const std::string& path) {
    try {
        return YAML::LoadFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading cars_config file: " << e.what() << std::endl;
        throw;
    }
}


Car CarBuilder::create_car(const std::string& path, const int& id, const int& car_type) {
    try {
        YAML::Node cars_data = open_file(path);
        if (!cars_data) {
            throw std::runtime_error("Invalid YAML archive: " + path);
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
                   car_width);
    } catch (const std::exception& e) {
        std::cerr << "Error building the car: " << e.what() << std::endl;
        throw;
    }
}
