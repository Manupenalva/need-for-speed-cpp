#ifndef CARBUILDER_H
#define CARBUILDER_H


#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "car.h"


class CarBuilder {
public:
    YAML::Node cars_data;

    explicit CarBuilder(const std::string& path);
    Car create_car(const int& id, const int& car_type);
    // std::vector<CarProperties> get_catalog();

private:
    CarBuilder(const CarBuilder&) = delete;
    CarBuilder& operator=(const CarBuilder&) = delete;
};

#endif
