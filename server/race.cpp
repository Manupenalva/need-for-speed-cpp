#include "race.h"

#include <string>

#include "mapCollisionBuilder.h"

Race::Race(std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars,
           const float& celd_width, const float& celd_height,
           const std::vector<Position>& start_positions, const Position& finish,
           const std::vector<Position>& checkpoints, const std::string& map_path):
        players_cars(players_cars),
        celd_width(celd_width),
        celd_height(celd_height),
        start_positions(start_positions),
        finish(finish),
        checkpoints(checkpoints),
        map_collisions_path(map_path),
        world() {}

b2WorldId Race::start_race() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    world = b2CreateWorld(&worldDef);

    MapCollisionBuilder::initialize_map_buildings(map_collisions_path, world);

    int i = 0;
    for (const auto& [id, car]: players_cars) {
        car->add_to_world(world, start_positions[i], checkpoints[0]);
        i++;
    }

    return world;
}
