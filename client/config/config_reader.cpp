#include "config_reader.h"

ConfigReader::ConfigReader() {
    try {
        config = YAML::LoadFile(CONFIG_PATH);
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration file: " << e.what() << std::endl;
        throw;
    }
}

// Devuelve la instancia singleton de ConfigReader, si no esta creada se crea y la devuelve, si ya
// esta creada solo la devuelve
ConfigReader& ConfigReader::get_instance() {
    static ConfigReader instance;
    return instance;
}

int ConfigReader::get_window_width() { return config["window"]["width"].as<int>(); }

int ConfigReader::get_window_height() { return config["window"]["height"].as<int>(); }

int ConfigReader::get_map_id() { return config["map"]["id"].as<int>(); }

int ConfigReader::get_map_wide() { return config["map"]["wide"].as<int>(); }

int ConfigReader::get_music_volume() { return config["music"]["volume"].as<int>(); }
