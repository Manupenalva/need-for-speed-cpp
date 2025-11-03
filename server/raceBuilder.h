#ifndef RACEBUILDER_H
#define RACEBUILDER_H


#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include "race.h"


class RaceBuilder {
public:
    static std::unique_ptr<Race> create_race(
            const std::string& path,
            std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars);

private:
    static YAML::Node open_file(const std::string& path);
    static std::string get_map_collitions_path(const std::string& city_name);
    static float get_hint_angle(const std::string& rotation);

    RaceBuilder() = delete;
    RaceBuilder(const RaceBuilder&) = delete;
    RaceBuilder& operator=(const RaceBuilder&) = delete;
};

#endif
