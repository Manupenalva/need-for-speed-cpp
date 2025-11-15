#include "gameloop.h"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <thread>

#include "../common/gameLoop_timer.h"
#include "../libs/box2d/include/box2d/box2d.h"
#include "events/actionmessage.h"
#include "events/cheatmessage.h"
#include "events/selectcarmessage.h"
#include "../common/constants.h"

#include "carBuilder.h"
#include "carPhysics.h"
#include "mapCollisionBuilder.h"
#include "raceBuilder.h"
#define TARGET_FPS 60
#define TARGET_FPS_INTERVAL 30
#define INTERVAL_WAIT_TIME 10  // segundos
#define FRAME_INTERVAL_TO_CLOSE 300  // segundos

Gameloop::Gameloop(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
        std::shared_ptr<RaceStruct> race_monitor):
        user_commands_queue(user_commands_queue), race_monitor(race_monitor), races(), frames(0) {}

void Gameloop::update_car_input(const uint16_t& player_id, const uint8_t& action) {
    players_cars[player_id].update_input(action);
}

void Gameloop::upgrade_car_stats(const uint16_t& player_id, const uint8_t& action) {
    players_cars[player_id].upgrade_stats(action);
}

void Gameloop::broadcast_players(const int& race_index) {
    ServerMessageDTO msg = races[race_index]->get_broadcast_message(frames);

    race_monitor->broadcast(msg);
}

void Gameloop::broadcast_event(const MsgType msg_type) {
    ServerMessageDTO msg;
    msg.type = msg_type;
    race_monitor->broadcast(msg);
}

void Gameloop::broadcast_interval(const int& race_index) {
    ServerMessageDTO msg = races[race_index]->get_interval_message();
    race_monitor->broadcast(msg);
}

void Gameloop::broadcast_map_data(const uint8_t& city_code) {
    ServerMessageDTO msg;
    msg.type = MsgType::SEND_MAP_NUMBER;
    msg.map_number = city_code;
    race_monitor->broadcast(msg);
}

void Gameloop::broadcast_minimap_info(int race_index) {
    ServerMessageDTO msg;
    msg.type = MsgType::SEND_MINIMAP_INFO;

    MinimapInfo minimap_info;
    minimap_info.checkpoints = races[race_index]->get_checkpoints_info();
    minimap_info.arrows = races[race_index]->get_checkpoints_arrows();
    msg.minimap_info = minimap_info;
    race_monitor->broadcast(msg);
}

void Gameloop::initialize_races() {
    for (const auto& entry: std::filesystem::directory_iterator("../server/assets/race_configs")) {
        if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
            races.push_back(RaceBuilder::create_race(entry.path(), players_cars));
        }
    }
}

void Gameloop::receive_selected_cars() {
    CarBuilder builder("../server/assets/cars_configs/cars_config.yaml");
    while (should_keep_running()) {
        std::shared_ptr<ClientHandlerMessage> base_msg;

        while (user_commands_queue->try_pop(base_msg)) {

            if (base_msg->get_msg_type() != MsgType::SELECT_CAR) {
                continue;
            }
            std::shared_ptr<SelectCarMessage> msg =
                    std::static_pointer_cast<SelectCarMessage>(base_msg);

            int player_id = msg->get_client_id();
            int selected_car = msg->get_car_id();

            players_cars[player_id] = builder.create_car(player_id, selected_car);

            if (static_cast<int>(players_cars.size()) >= race_monitor->size()) {
                return;
            }
        }
    }
}

void Gameloop::handle_upgrades_phase(const int& race_index) {
    GameLoopTimer timer(TARGET_FPS);
    int frames_interval = 0;
    uint32_t iterations_behind = 1;
    while (should_keep_running() && frames_interval < FRAME_INTERVAL_TO_CLOSE) {
        std::shared_ptr<ClientHandlerMessage> base_msg;
        while (user_commands_queue->try_pop(base_msg) && should_keep_running()) {
            if (base_msg->get_msg_type() != MsgType::DRIVING_EVENT) {
                continue;
            }
            std::shared_ptr<ActionMessage> msg = std::static_pointer_cast<ActionMessage>(base_msg);
            for (const auto& action: msg->get_actions()) {
                upgrade_car_stats(msg->get_client_id(), action);
            }
        }
        broadcast_interval(race_index);
        frames_interval++;
        timer.sleep_and_calc_next_it(iterations_behind);
    }

    broadcast_event(MsgType::INTERVAL_CLOSED);
}

void Gameloop::handle_race(const int& race_index) {
    if (race_index != 0) {
        std::this_thread::sleep_for(std::chrono::seconds(INTERVAL_WAIT_TIME));
    }
    broadcast_event(MsgType::RACE_STARTED);
    uint8_t city_code = races[race_index]->get_city_code();
    broadcast_map_data(city_code);
    broadcast_minimap_info(race_index);
    frames = 0;
    GameLoopTimer timer(TARGET_FPS);
    uint32_t iterations_behind = 1;
    races[race_index]->start_race();
    broadcast_players(race_index); //Posiciones iniciales
    broadcast_event(MsgType::RACE_COUNTDOWN);
    bool accepting_inputs = false;
    std::chrono::steady_clock::time_point countdown_start_time = std::chrono::steady_clock::now();
    
    while (!races[race_index]->is_finished() && should_keep_running()) {
        if (std::chrono::steady_clock::now() - countdown_start_time >= std::chrono::seconds(COUNTDOWN_TIME) && !accepting_inputs) {
            broadcast_event(MsgType::COUNTDOWN_FINISHED);
            accepting_inputs = true;
        }
        std::shared_ptr<ClientHandlerMessage> base_msg;
        while (user_commands_queue->try_pop(base_msg)) {
            if (!accepting_inputs) {
                continue;
            }
            if (base_msg->get_msg_type() == MsgType::DRIVING_EVENT) {
                std::shared_ptr<ActionMessage> msg =
                        std::static_pointer_cast<ActionMessage>(base_msg);
                for (const auto& action: msg->get_actions()) {
                    update_car_input(msg->get_client_id(), action);
                }
            } else if (base_msg->get_msg_type() == MsgType::CHEAT_CODE) {
                std::shared_ptr<CheatMessage> msg =
                        std::static_pointer_cast<CheatMessage>(base_msg);
                handle_cheat_code(msg->get_client_id(), msg->get_cheat_code(), race_index);
            }
        }

        if (accepting_inputs){
            for (uint32_t i = 0; i < iterations_behind; i++) {
                races[race_index]->update_state();
            }
        }
        broadcast_players(race_index);
        frames++;
        timer.sleep_and_calc_next_it(iterations_behind);
    }

    broadcast_event(MsgType::RACE_FINISHED);
    if (race_index == static_cast<int>(races.size()) - 1)
        return;
    handle_upgrades_phase(race_index);
}

void Gameloop::handle_cheat_code(const uint16_t& player_id, const CheatCode& cheat_code,
                                 int race_index) {
    switch (cheat_code) {
        case CheatCode::INFINITE_HEALTH:
            players_cars[player_id].activate_infinite_health();
            break;
        case CheatCode::WIN_AUTOMATICALLY:
            races[race_index]->force_finish_race(player_id);
            break;
        case CheatCode::LOSE_AUTOMATICALLY:
            races[race_index]->force_lose_race(player_id);
            break;
        case CheatCode::MAX_STATS:
            players_cars[player_id].maximize_stats();
            break;
        default:
            break;
    }
}

void Gameloop::run() {
    broadcast_event(MsgType::GAME_START);
    receive_selected_cars();
    initialize_races();
    for (size_t i = 0; i < races.size(); i++) {
        handle_race(i);
        if (!should_keep_running())
            return;
    }
    broadcast_event(MsgType::GAME_END);
}
