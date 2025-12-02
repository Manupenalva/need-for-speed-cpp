#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H


#include <memory>
#include <random>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "../common/carProperties.h"
#include "race_logic/car.h"
#include "race_logic/carConstants.h"
#include "race_logic/configConstants.h"
#include "race_logic/npc.h"


class ConfigLoader {
private:
    YAML::Node config_data;

public:
    explicit ConfigLoader(const std::string& path);
    std::shared_ptr<ConfigConstants> load_constants();

private:
    ConfigLoader(const ConfigLoader&) = delete;
    ConfigLoader& operator=(const ConfigLoader&) = delete;
};

#endif
