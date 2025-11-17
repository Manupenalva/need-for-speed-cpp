#include "mapCollisionBuilder.h"

#include <vector>

#define MAX_CORNER_DISTANCE 600.0f

YAML::Node MapCollisionBuilder::open_file(const std::string& path) {
    try {
        return YAML::LoadFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading map collision file: " << e.what() << std::endl;
        throw;
    }
}

float MapCollisionBuilder::RayCastCallback(b2ShapeId /*shapeId*/, b2Vec2 /*point*/,
                                           b2Vec2 /*normal*/, float /*fraction*/, void* context) {
    bool* hit = static_cast<bool*>(context);
    *hit = true;
    return 0.0f;
}

void MapCollisionBuilder::connect_corners(std::vector<Corner>& corners, b2WorldId world) {
    b2QueryFilter filter = b2DefaultQueryFilter();

    for (Corner& corner_1: corners) {
        for (Corner& corner_2: corners) {
            if (corner_1.id == corner_2.id)
                continue;

            float x_distance = (float)(corner_2.position.x - corner_1.position.x);
            float y_distance = (float)(corner_2.position.y - corner_1.position.y);
            float distance = std::sqrt(x_distance * x_distance + y_distance * y_distance);
            if (distance > MAX_CORNER_DISTANCE)
                continue;

            b2Vec2 origin = {(float)corner_1.position.x, (float)corner_1.position.y};
            b2Vec2 translation = {x_distance, y_distance};

            bool hit = false;
            b2World_CastRay(world, origin, translation, filter, RayCastCallback, &hit);
            if (!hit) {
                corner_1.neighbors.push_back(corner_2.id);
            }
        }
    }
}

b2ShapeId MapCollisionBuilder::create_body(bool is_sensor, float x, float y, float width,
                                           float height, b2WorldId world) {
    float new_width = width;
    if (is_sensor)
        new_width = width - 5.0f;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = {x + (new_width / 2.0f), y + (height / 2.0f)};
    b2BodyId body = b2CreateBody(world, &bodyDef);
    b2Body_EnableContactEvents(body, true);
    b2Body_EnableHitEvents(body, true);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    if (is_sensor)
        shapeDef.isSensor = true;
    b2Polygon box = b2MakeBox((new_width / 2.0f), (height / 2.0f));
    b2ShapeId shape = b2CreatePolygonShape(body, &shapeDef, &box);
    b2Shape_EnableSensorEvents(shape, true);
    b2Shape_EnableContactEvents(shape, true);
    b2Shape_EnableHitEvents(shape, true);

    return shape;
}

MapData MapCollisionBuilder::initialize_map_buildings(const std::string& path, b2WorldId world) {
    try {
        YAML::Node map_collisions = open_file(path);
        if (!map_collisions) {
            std::cerr << "Invalid YAML archive: " << path << std::endl;
            return {};
        }

        int bridges_amount = 0;
        for (const auto& properties: map_collisions["properties"]) {
            if (properties["name"].as<std::string>() == "bridges_amount") {
                bridges_amount =                        // cppcheck-suppress useStlAlgorithm
                        properties["value"].as<int>();  // cppcheck-suppress useStlAlgorithm
            }
        }
        std::cout << bridges_amount << std::endl;

        std::vector<Bridge> bridges(bridges_amount);
        MapData map_data;

        for (const auto& layer: map_collisions["layers"]) {
            if (layer["name"].as<std::string>() == "Colisiones") {
                for (const auto& object: layer["objects"]) {
                    float x = object["x"].as<float>() - 25.0f;
                    float y = object["y"].as<float>() - 25.0f;
                    float width = object["width"].as<float>();
                    float height = object["height"].as<float>();

                    create_body(false, x, y, width, height, world);
                }
            }

            if (layer["name"].as<std::string>() == "Puentes") {
                for (const auto& object: layer["objects"]) {

                    int bridge_id = object["type"].as<int>();
                    std::string side = object["name"].as<std::string>();

                    float x = object["x"].as<float>() - 25.0f;
                    float y = object["y"].as<float>() - 25.0f;
                    float width = object["width"].as<float>();
                    float height = object["height"].as<float>();

                    b2ShapeId sensor = create_body(true, x, y, width, height, world);

                    if (side == "up 1")
                        bridges[bridge_id].sensor1_up = sensor;
                    else if (side == "up 2")
                        bridges[bridge_id].sensor2_up = sensor;
                    else if (side == "down 1")
                        bridges[bridge_id].sensor1_down = sensor;
                    else if (side == "down 2")
                        bridges[bridge_id].sensor2_down = sensor;
                }
            }


            if (layer["name"].as<std::string>() == "Esquinas") {
                int current_corner_id = 0;
                for (const auto& object: layer["objects"]) {
                    Corner corner;
                    corner.id = current_corner_id;
                    corner.position.x = object["x"].as<float>() - 25.0f;
                    corner.position.y = object["y"].as<float>() - 25.0f;
                    map_data.corners.push_back(corner);
                    current_corner_id++;
                }
                connect_corners(map_data.corners, world);
            }

            if (layer["name"].as<std::string>() == "Npcs") {
                for (const auto& object: layer["objects"]) {
                    Position position;
                    position.x = object["x"].as<float>() - 25.0f;
                    position.y = object["y"].as<float>() - 25.0f;
                    map_data.spawn_positions.push_back(position);
                }
            }
        }
        map_data.bridges = bridges;

        return map_data;
    } catch (const std::exception& e) {
        std::cerr << "Error building the map with box2d: " << e.what() << std::endl;
        throw;
    }
}
