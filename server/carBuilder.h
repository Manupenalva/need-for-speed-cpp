#ifndef CARBUILDER_H
#define CARBUILDER_H


#include <string>

#include <yaml-cpp/yaml.h>

#include "car.h"


class CarBuilder {
public:
    static Car create_car(const std::string& path, const int& id, const int& car_type);

private:
    static YAML::Node open_file(const std::string& path);

    CarBuilder() = delete;
    CarBuilder(const CarBuilder&) = delete;
    CarBuilder& operator=(const CarBuilder&) = delete;
};

#endif
