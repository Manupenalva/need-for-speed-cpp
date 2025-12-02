#ifndef CARBUILDER_H
#define CARBUILDER_H


#include <memory>
#include <random>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "../../common/carProperties.h"

#include "car.h"
#include "carConstants.h"
#include "npc.h"


class CarBuilder {
private:
    YAML::Node cars_data;
    std::shared_ptr<CarConstants> car_constants;
    std::mt19937 random_seed;

public:
    explicit CarBuilder(const std::string& path);
    CarBuilder(const std::string& path, std::shared_ptr<CarConstants> constants);
    Car create_car(const int& id, const int& car_type);
    std::unique_ptr<Npc> create_npc(const Position& position, std::vector<Corner>& corners,
                                    b2WorldId world);
    std::vector<CarProperties> get_catalog();

private:
    CarBuilder(const CarBuilder&) = delete;
    CarBuilder& operator=(const CarBuilder&) = delete;
    void load_constants();
};

#endif
