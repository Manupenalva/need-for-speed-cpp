#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <iostream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "constants.h"

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
    int get_player_car();
    int get_map_id();
};

#endif
