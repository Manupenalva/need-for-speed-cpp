#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <box2d/box2d.h>

#include "../common/carState.h"
#include "../common/cheatcodes.h"
#include "../common/queue.h"
#include "../common/thread.h"
#include "events/clienthandlermessage.h"

#include "car.h"
#include "carConstants.h"
#include "race.h"
#include "racemonitor.h"


class Gameloop: public Thread {
private:
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue;
    std::shared_ptr<RaceStruct> race_monitor;
    std::unordered_map<uint16_t, Car> players_cars;
    std::vector<std::unique_ptr<Race>> races;
    uint32_t frames;
    int countdown_remaining;
    std::shared_ptr<CarConstants> car_constants;
    std::unordered_map<int, std::string> player_usernames;

public:
    Gameloop(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
             std::shared_ptr<RaceStruct> race_monitor);
    void run() override;

private:
    void initialize_races();
    void broadcast_players(const int& race_index);
    void update_positions();
    void update_car_input(const uint16_t& player_id, const uint8_t& action);
    void receive_selected_cars();
    void broadcast_event(const MsgType msg_type);
    void handle_upgrades_phase(const int& race_index);
    void handle_race(const int& race_index);
    void upgrade_car_stats(const uint16_t& player_id, const uint8_t& action);
    void broadcast_interval(const int& race_index);
    void handle_cheat_code(const uint16_t& player_id, const CheatCode& cheat_code, int race_index);
    void broadcast_map_data(const uint8_t& city_code);
    void broadcast_minimap_info(int race_index);
    void broadcast_positions(int race_index);
    void handle_countdown(int race_index);
    std::vector<ResultInfo> get_acumullated_times();
    std::vector<ResultInfo> get_positions_message(
            const std::vector<std::tuple<uint16_t, float, float>>& positions);

    Gameloop(const Gameloop& other) = delete;
    Gameloop& operator=(const Gameloop& other) = delete;
};

#endif
