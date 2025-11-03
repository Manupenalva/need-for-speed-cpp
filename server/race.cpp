#include "race.h"

#include <string>

#include "mapCollisionBuilder.h"

Race::Race(std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars,
           const float& celd_width, const float& celd_height,
           const std::vector<Position>& start_positions, const Position& finish,
           const std::vector<Position>& checkpoints, const std::string& map_path):
        players_cars(players_cars),
        players_status(),
        celd_width(celd_width),
        celd_height(celd_height),
        start_positions(start_positions),
        checkpoints(checkpoints),
        map_collisions_path(map_path),
        current_time(0.0f),
        world() {
    this->checkpoints.push_back(finish);
}

b2WorldId Race::start_race() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    world = b2CreateWorld(&worldDef);

    MapCollisionBuilder::initialize_map_buildings(map_collisions_path, world);

    int i = 0;
    for (const auto& [id, car]: players_cars) {
        car->add_to_world(world, start_positions[i]);
        players_status[id] = {false, 0};
        i++;
    }

    return world;
}

void Race::update_state() {
    for (auto& [id, car]: players_cars) {
        PlayerRaceStatus& status = players_status[id];

        if (status.has_finished)
            continue;

        Position next_checkpoint = checkpoints[status.current_checkpoint_index];
        if (car->reached_checkpoint(next_checkpoint, celd_width, celd_height)) {
            status.current_checkpoint_index++;
            if (status.current_checkpoint_index >= checkpoints.size()) {
                std::cout << "terminé la carrera" << std::endl;
                status.has_finished = true;
            }
        }

        car->update_physics();
    }

    float timeStep = 1.0f / 60.0f;
    b2World_Step(world, timeStep, 3);

    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished)
            car->update_position();
    }
    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished)
            car->handle_hits();
    }
}
