#ifndef MAPCOLLISIONBUILDER_H
#define MAPCOLLISIONBUILDER_H

#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "../libs/box2d/include/box2d/box2d.h"

#include "corner.h"


struct NpcsData {
    std::vector<Corner> corners;
    std::vector<Position> spawn_positions;
};


class MapCollisionBuilder {
public:
    static NpcsData initialize_map_buildings(const std::string& path, b2WorldId world);

private:
    static YAML::Node open_file(const std::string& path);
    static void connect_corners(std::vector<Corner>& corners, b2WorldId world);
    static float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction,
                                 void* context);

    MapCollisionBuilder() = delete;
    MapCollisionBuilder(const MapCollisionBuilder&) = delete;
    MapCollisionBuilder& operator=(const MapCollisionBuilder&) = delete;
};

#endif
