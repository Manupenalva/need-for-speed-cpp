#include "mapCollisionBuilder.h"

#include <vector>

YAML::Node MapCollisionBuilder::open_file(const std::string& path) {
    try {
        return YAML::LoadFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading map collision file: " << e.what() << std::endl;
        throw;
    }
}

void MapCollisionBuilder::initialize_map_buildings(const std::string& path, b2WorldId world) {
    try {
        YAML::Node map_collisions = open_file(path);
        if (!map_collisions) {
            std::cerr << "Invalid YAML archive: " << path << std::endl;
            return;
        }

        for (const auto& layer: map_collisions["layers"]) {
            if (layer["name"].as<std::string>() == "Colisiones") {
                for (const auto& object: layer["objects"]) {
                    float x = object["x"].as<float>() - 25.0f;
                    float y = object["y"].as<float>() - 25.0f;
                    float width = object["width"].as<float>();
                    float height = object["height"].as<float>();

                    b2BodyDef bodyDef = b2DefaultBodyDef();
                    bodyDef.type = b2_staticBody;
                    bodyDef.position = {x + (width / 2.0f), y + (height / 2.0f)};
                    b2BodyId body = b2CreateBody(world, &bodyDef);
                    b2Body_EnableContactEvents(body, true);
                    b2Body_EnableHitEvents(body, true);

                    b2ShapeDef shapeDef = b2DefaultShapeDef();
                    b2Polygon box = b2MakeBox((width / 2.0f), (height / 2.0f));
                    b2ShapeId shape = b2CreatePolygonShape(body, &shapeDef, &box);
                    b2Shape_EnableContactEvents(shape, true);
                    b2Shape_EnableHitEvents(shape, true);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error building the map with box2d: " << e.what() << std::endl;
        throw;
    }
}
