#include "configLoader.h"

ConfigLoader::ConfigLoader(const std::string& path): config_data(YAML::LoadFile(path)) {}

std::shared_ptr<ConfigConstants> ConfigLoader::load_constants() {

    if (config_data.IsNull()) {
        throw std::runtime_error("Failed to load configuration file.");
    }
    auto constants = std::make_shared<ConfigConstants>();
    auto config_times = config_data["times"];
    auto config_rates = config_data["rates"];
    if (config_times.IsNull() || config_rates.IsNull()) {
        throw std::runtime_error("Configuration file is missing required sections.");
    }

    constants->countdown_time = config_times["countdown"].as<int>();
    constants->race_duration = config_times["race_duration"].as<float>();
    constants->positions_wait_time = config_times["positions"].as<int>();
    constants->interval_iters = config_times["interval_iters"].as<int>();
    constants->interval_wait_time = config_times["interval_wait_time"].as<int>();

    constants->game_rate = config_rates["game_rate"].as<int>();
    constants->positions_rate = config_rates["positions_rate"].as<int>();
    constants->interval_rate = config_rates["interval_rate"].as<int>();

    constants->race_amount = config_data["race_amount"].as<int>();

    return constants;
}
