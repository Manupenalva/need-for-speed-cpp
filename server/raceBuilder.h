#ifndef RACEBUILDER_H
#define RACEBUILDER_H


#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include "configConstants.h"
#include "race.h"


class RaceBuilder {
public:
    static std::unique_ptr<Race> create_race(const std::string& path,
                                             std::unordered_map<uint16_t, Car>& players_cars,
                                             const std::shared_ptr<ConfigConstants>& configs);

private:
    static YAML::Node open_file(const std::string& path);
    static std::string get_map_data(const std::string& city_name, uint8_t& city_code);
    static float get_hint_angle(const std::string& rotation);
    static float get_checkpoint_angle(const std::string& rotation);

    RaceBuilder() = delete;
    RaceBuilder(const RaceBuilder&) = delete;
    RaceBuilder& operator=(const RaceBuilder&) = delete;
};

#endif
