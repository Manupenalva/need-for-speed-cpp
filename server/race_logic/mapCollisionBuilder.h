#ifndef MAPCOLLISIONBUILDER_H
#define MAPCOLLISIONBUILDER_H

#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "../../libs/box2d/include/box2d/box2d.h"

#include "bridge.h"
#include "corner.h"


struct MapData {
    std::vector<Corner> corners;
    std::vector<Position> spawn_positions;
    std::vector<Bridge> bridges;
};


class MapCollisionBuilder {
public:
    static MapData initialize_map_buildings(const std::string& path, b2WorldId world);

private:
    static YAML::Node open_file(const std::string& path);
    static void connect_corners(std::vector<Corner>& corners, b2WorldId world);
    static float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction,
                                 void* context);
    static b2ShapeId create_body(bool is_sensor, float x, float y, float width, float height,
                                 b2WorldId world);

    MapCollisionBuilder() = delete;
    MapCollisionBuilder(const MapCollisionBuilder&) = delete;
    MapCollisionBuilder& operator=(const MapCollisionBuilder&) = delete;
};

#endif
