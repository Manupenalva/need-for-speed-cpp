#ifndef RACE_H
#define RACE_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../common/carState.h"


class Race {
private:
    std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars;
    std::vector<std::unique_ptr<Car>> npcs;
    float cell_width;
    float cell_height;
    Position start;
    Position finish;
    std::vector<Position> checkpoints;

public:
    Race(std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars, const float& cell_width,
         const float& cell_height, const Position& start, const Position& finish,
         const std::vector<Position>& checkpoints);
    void start_race();

private:
    Race(const Race& other) = delete;
    Race& operator=(const Race& other) = delete;
};

#endif
