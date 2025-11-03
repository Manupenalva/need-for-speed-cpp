#include "raceBuilder.h"

#include <vector>

#include "mapCollisionBuilder.h"


#define LIBERTY_CITY_MAP "Backgrounds - Liberty City"
#define VICE_CITY_MAP "Backgrounds - Vice City"
#define SAN_ANDREAS_MAP "Backgrounds - San Andreas"

#define LIBERTY_CITY_PATH "../server/assets/maps_buildings/LibertyCityCollitions.yaml"
#define VICE_CITY_PATH "../server/assets/maps_buildings/ViceCityCollitions.yaml"
#define SAN_ANDREAS_PATH "../server/assets/maps_buildings/SanAndreasCollitions.yaml"


YAML::Node RaceBuilder::open_file(const std::string& path) {
    try {
        return YAML::LoadFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading race_config file: " << e.what() << std::endl;
        throw;
    }
}

std::string RaceBuilder::get_map_collitions_path(const std::string& city_name) {
    if (city_name == LIBERTY_CITY_MAP) {
        return LIBERTY_CITY_PATH;
    } else if (city_name == VICE_CITY_MAP) {
        return VICE_CITY_PATH;
    } else {
        return SAN_ANDREAS_PATH;
    }
}

std::unique_ptr<Race> RaceBuilder::create_race(
        const std::string& path, std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars) {
    try {
        YAML::Node race_data = open_file(path);
        if (!race_data) {
            throw std::runtime_error("Invalid YAML archive: " + path);
        }

        std::string city_name = race_data["city"].as<std::string>();
        std::string map_collitions_path = get_map_collitions_path(city_name);

        float celd_width = race_data["celdWidth"].as<float>();
        float celd_height = race_data["celdHeight"].as<float>();

        YAML::Node finish_node = race_data["finish"][0];
        Position finish;
        finish.x = finish_node["x"].as<float>();
        finish.y = finish_node["y"].as<float>();

        std::vector<Position> start_positions;
        for (const auto& position: race_data["start"]) {
            int x = position["x"].as<int>();
            int y = position["y"].as<int>();
            start_positions.push_back({x, y});
        }

        std::vector<Position> checkpoints;
        for (const auto& position: race_data["checkpoint"]) {
            int x = position["x"].as<int>();
            int y = position["y"].as<int>();
            checkpoints.push_back({x, y});
        }

        return std::make_unique<Race>(players_cars, celd_width, celd_height, start_positions,
                                      finish, checkpoints, map_collitions_path);

    } catch (const std::exception& e) {
        std::cerr << "Error building the race: " << e.what() << std::endl;
        throw;
    }
}
