#include "raceBuilder.h"

#include <vector>

#include "hint.h"
#include "mapCollisionBuilder.h"


#define LIBERTY_CITY_MAP "Backgrounds - Liberty City"
#define VICE_CITY_MAP "Backgrounds - Vice City"
#define SAN_ANDREAS_MAP "Backgrounds - San Andreas"

#define LIBERTY_CITY_CODE 0x01
#define SAN_ANDREAS_CODE 0x02
#define VICE_CITY_CODE 0x03

#define LIBERTY_CITY_PATH "../server/assets/maps_buildings/LibertyCityCollitions.yaml"
#define VICE_CITY_PATH "../server/assets/maps_buildings/ViceCityCollitions.yaml"
#define SAN_ANDREAS_PATH "../server/assets/maps_buildings/SanAndreasCollitions.yaml"

#define UP_ROTATION "up"
#define DOWN_ROTATION "down"
#define RIGHT_ROTATION "right"
#define LEFT_ROTATION "left"
#define HORIZONTAL_ROTATION "horizontal"
#define VERTICAL_ROTATION "vertical"


YAML::Node RaceBuilder::open_file(const std::string& path) {
    try {
        return YAML::LoadFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading race_config file: " << e.what() << std::endl;
        throw;
    }
}

std::string RaceBuilder::get_map_data(const std::string& city_name, uint8_t& city_code) {
    if (city_name == LIBERTY_CITY_MAP) {
        city_code = LIBERTY_CITY_CODE;
        return LIBERTY_CITY_PATH;
    } else if (city_name == VICE_CITY_MAP) {
        city_code = VICE_CITY_CODE;
        return VICE_CITY_PATH;
    } else {
        city_code = SAN_ANDREAS_CODE;
        return SAN_ANDREAS_PATH;
    }
}

float RaceBuilder::get_start_angle(const std::string& rotation) {
    if (rotation == UP_ROTATION) {
        return 270.0f;
    } else if (rotation == DOWN_ROTATION) {
        return 90.0f;
    } else if (rotation == RIGHT_ROTATION) {
        return 180.0f;
    } else {
        return 0.0f;
    }
}

float RaceBuilder::get_hint_angle(const std::string& rotation) {
    if (rotation == UP_ROTATION) {
        return 270.0f;
    } else if (rotation == DOWN_ROTATION) {
        return 90.0f;
    } else if (rotation == RIGHT_ROTATION) {
        return 0.0f;
    } else {
        return 180.0f;
    }
}

float RaceBuilder::get_checkpoint_angle(const std::string& rotation) {
    if (rotation == HORIZONTAL_ROTATION) {
        return 0.0f;
    } else {
        return 90.0f;
    }
}

std::unique_ptr<Race> RaceBuilder::create_race(const std::string& path,
                                               std::unordered_map<uint16_t, Car>& players_cars,
                                               const std::shared_ptr<ConfigConstants>& configs) {
    try {
        YAML::Node race_data = open_file(path);
        if (!race_data) {
            throw std::runtime_error("Invalid YAML archive: " + path);
        }

        std::string city_name = race_data["city"].as<std::string>();
        uint8_t city_code;
        std::string map_collitions_path = get_map_data(city_name, city_code);

        float celd_width = race_data["celdWidth"].as<float>();
        float celd_height = race_data["celdHeight"].as<float>();

        YAML::Node finish_node = race_data["finish"][0];
        Position finish;
        finish.x = finish_node["x"].as<float>();
        finish.y = finish_node["y"].as<float>();
        finish.angle = get_checkpoint_angle(finish_node["rotation"].as<std::string>());

        std::vector<Position> start_positions;
        for (const auto& position: race_data["start"]) {
            int x = position["x"].as<int>();
            int y = position["y"].as<int>();
            std::string rotation = position["rotation"].as<std::string>();
            start_positions.push_back({x, y, get_start_angle(rotation)});
        }

        std::vector<Position> checkpoints;
        for (const auto& position: race_data["line"]) {
            int x = position["x"].as<int>();
            int y = position["y"].as<int>();
            std::string rotation = position["rotation"].as<std::string>();
            checkpoints.push_back({x, y, get_checkpoint_angle(rotation)});
        }

        for (const auto& position: race_data["checkpoint"]) {
            int x = position["x"].as<int>();
            int y = position["y"].as<int>();
            std::string rotation = position["rotation"].as<std::string>();
            checkpoints.push_back({x, y, get_checkpoint_angle(rotation)});
        }

        std::unordered_map<int, std::vector<Position>> hints;
        for (const auto& hint: race_data["hint"]) {
            int x = hint["x"].as<int>();
            int y = hint["y"].as<int>();
            std::string rotation = hint["rotation"].as<std::string>();
            float angle = get_hint_angle(rotation);

            int checkpoint_id = hint["id"].as<int>();
            Position pos = {x, y, angle};

            hints[checkpoint_id].push_back(pos);
        }

        return std::make_unique<Race>(players_cars, celd_width, celd_height, start_positions,
                                      finish, checkpoints, hints, map_collitions_path, city_code,
                                      configs->race_duration);

    } catch (const std::exception& e) {
        std::cerr << "Error building the race: " << e.what() << std::endl;
        throw;
    }
}
