#ifndef MAPCOLLISIONBUILDER_H
#define MAPCOLLISIONBUILDER_H

#include <iostream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "../libs/box2d/include/box2d/box2d.h"


class MapCollisionBuilder {
public:
    static void initialize_map_buildings(const std::string& path, b2WorldId world);

private:
    static YAML::Node open_file(const std::string& path);

    MapCollisionBuilder() = delete;
    MapCollisionBuilder(const MapCollisionBuilder&) = delete;
    MapCollisionBuilder& operator=(const MapCollisionBuilder&) = delete;
};

#endif
