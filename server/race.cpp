#include "race.h"

Race::Race(std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars,
           const float& cell_width, const float& cell_height, const Position& start,
           const Position& finish, const std::vector<Position>& checkpoints):
        players_cars(players_cars),
        cell_width(cell_width),
        cell_height(cell_height),
        start(start),
        finish(finish),
        checkpoints(checkpoints) {}

void Race::start_race() {
    // setear posision inicial y proximo checkpoint de los autos.
}
