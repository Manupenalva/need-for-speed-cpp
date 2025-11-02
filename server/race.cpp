#include "race.h"

#include <string>

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
        map_collisions_path(map_path) {}

b2WorldId Race::start_race() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    return b2CreateWorld(&worldDef);
    // crear world, setear posision inicial de los autos y proximo checkpoint de los autos.
}
