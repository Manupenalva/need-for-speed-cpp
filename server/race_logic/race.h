#ifndef RACE_H
#define RACE_H

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../common/carState.h"
#include "../../common/messageDTOs.h"
#include "../../common/resultInfo.h"
#include "../../libs/box2d/include/box2d/box2d.h"

#include "car.h"
#include "corner.h"
#include "npc.h"
#include "playerRaceStatus.h"

class Race {
private:
    std::unordered_map<uint16_t, Car>& players_cars;
    std::vector<std::unique_ptr<Npc>> npcs;
    std::unordered_map<uint16_t, PlayerRaceStatus> players_status;
    float celd_width;
    float celd_height;
    std::vector<Corner> corners;
    std::vector<Bridge> bridges;
    std::vector<Position> start_positions;
    std::vector<Position> checkpoints;
    std::unordered_map<int, std::vector<Position>> hints;
    std::string map_collisions_path;
    float current_time;
    b2WorldId world;
    std::vector<std::tuple<uint16_t, float, float>> race_results;
    uint8_t city_code;
    float max_time;

public:
    Race(std::unordered_map<uint16_t, Car>& players_cars, const float& celd_width,
         const float& celd_height, const std::vector<Position>& start_positions,
         const Position& finish, const std::vector<Position>& checkpoints,
         std::unordered_map<int, std::vector<Position>>& hints, const std::string& map_path,
         uint8_t city_code, float max_time);
    b2WorldId start_race();
    void update_state();
    bool is_finished();
    ServerMessageDTO get_broadcast_message(float frames);
    CheckpointInfo get_next_checkpoint_info(const uint16_t car_id);
    CheckpointArrow get_next_checkpoint_arrow(const uint16_t car_id);
    const std::vector<std::tuple<uint16_t, float, float>> get_race_results();
    ServerMessageDTO get_interval_message();
    void force_finish_race(const uint16_t& player_id);
    void force_lose_race(const uint16_t& player_id);
    uint8_t get_city_code();
    std::vector<CheckpointInfo> get_checkpoints_info();
    std::vector<CheckpointArrow> get_checkpoints_arrows();

    ~Race();

private:
    void handle_sensors();
    void handle_bridge_interactions(b2ShapeId sensor_shape, const Bridge* bridge, Car* car);
    void correct_bridge_interactions(b2ShapeId sensor_shape, const Bridge* bridge, Car* car);

    Race(const Race& other) = delete;
    Race& operator=(const Race& other) = delete;

    void finish_race();
};
#endif
