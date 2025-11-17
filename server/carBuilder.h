#ifndef CARBUILDER_H
#define CARBUILDER_H


#include <memory>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "../common/carProperties.h"

#include "car.h"
#include "carConstants.h"


class CarBuilder {
public:
    YAML::Node cars_data;
    std::shared_ptr<CarConstants> car_constants;

    explicit CarBuilder(const std::string& path);
    CarBuilder(const std::string& path, std::shared_ptr<CarConstants> constants);
    Car create_car(const int& id, const int& car_type);
    std::vector<CarProperties> get_catalog();

private:
    CarBuilder(const CarBuilder&) = delete;
    CarBuilder& operator=(const CarBuilder&) = delete;
    void load_constants();
};

#endif
