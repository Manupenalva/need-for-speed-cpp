#ifndef RACE_H
#define RACE_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/carState.h"
#include "../common/messageDTOs.h"
#include "../libs/box2d/include/box2d/box2d.h"

#include "car.h"
#include "hint.h"
#include "playerRaceStatus.h"

class Race {
private:
    std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars;
    std::unordered_map<uint16_t, PlayerRaceStatus> players_status;
    float celd_width;
    float celd_height;
    std::vector<Position> start_positions;
    std::vector<Position> checkpoints;
    std::vector<Hint> hints;
    std::string map_collisions_path;
    float current_time;
    b2WorldId world;

public:
    Race(std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars, const float& celd_width,
         const float& celd_height, const std::vector<Position>& start_positions,
         const Position& finish, const std::vector<Position>& checkpoints,
         const std::vector<Hint>& hints, const std::string& map_path);
    b2WorldId start_race();
    void update_state();
    bool is_finished();
    ServerMessageDTO get_broadcast_message(float frames);
    CheckpointInfo get_next_checkpoint_info(const uint16_t car_id);
    CheckpointArrow get_next_checkpoint_arrow(const uint16_t car_id);

private:
    Race(const Race& other) = delete;
    Race& operator=(const Race& other) = delete;
};

#endif
