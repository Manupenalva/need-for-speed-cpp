#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "constants.h"

struct Bridge {
    int x;
    int y;
    int width;
    int height;
};

const std::map<MapType, std::string> BRIDGE_MAP = {
        {MapType::LIBERTY_CITY, "Liberty City"},
        {MapType::SAN_ANDREAS, "San Andreas"},
        {MapType::VICE_CITY, "Vice City"},
};

class ConfigReader {
private:
    YAML::Node config;

    ConfigReader();

public:
    static ConfigReader& get_instance();

    ConfigReader(const ConfigReader&) = delete;
    void operator=(const ConfigReader&) = delete;

    // Métodos para obtener tipos de valores especificos
    int get_window_width();
    int get_window_height();
    int get_map_id();
    int get_map_wide();
    int get_music_volume();
    std::map<MapType, std::vector<Bridge>> get_bridges_for_map();
};

#endif
