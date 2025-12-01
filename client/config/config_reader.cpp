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

std::map<MapType, std::vector<Bridge>> ConfigReader::get_bridges_for_map() {
    std::map<MapType, std::vector<Bridge>> bridges;
    try {
        for (const auto& [map_id, map_name]: BRIDGE_MAP) {
            YAML::Node bridges_node = config["map"]["bridges"][map_name];
            for (const auto& bridge_node: bridges_node) {
                Bridge bridge;
                bridge.x = bridge_node[0].as<int>();
                bridge.y = bridge_node[1].as<int>();
                bridge.width = bridge_node[2].as<int>();
                bridge.height = bridge_node[3].as<int>();
                bridges[map_id].push_back(bridge);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading bridges" << e.what() << std::endl;
    }
    return bridges;
}
