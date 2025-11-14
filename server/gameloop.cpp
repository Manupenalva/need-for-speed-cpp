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
#include "events/selectcarmessage.h"

#include "carBuilder.h"
#include "carPhysics.h"
#include "mapCollisionBuilder.h"
#include "raceBuilder.h"
#define TARGET_FPS 60
#define TARGET_FPS_INTERVAL 30
#define FRAME_INTERVAL_TO_CLOSE 300
#define CLOSED_INTERVAL_TIME 10  // segundos

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

    // broadcast_event(MsgType::INTERVAL_CLOSED);
}

void Gameloop::handle_race(const int& race_index) {
    broadcast_event(MsgType::RACE_STARTED);

    frames = 0;
    GameLoopTimer timer(TARGET_FPS);
    uint32_t iterations_behind = 1;
    races[race_index]->start_race();

    while (!races[race_index]->is_finished() && should_keep_running()) {
        std::shared_ptr<ClientHandlerMessage> base_msg;
        while (user_commands_queue->try_pop(base_msg)) {
            if (base_msg->get_msg_type() != MsgType::DRIVING_EVENT) {
                continue;
            }
            std::shared_ptr<ActionMessage> msg = std::static_pointer_cast<ActionMessage>(base_msg);
            for (const auto& action: msg->get_actions()) {
                update_car_input(msg->get_client_id(), action);
            }
        }

        for (uint32_t i = 0; i < iterations_behind; i++) {
            races[race_index]->update_state();
        }

        broadcast_players(race_index);
        frames++;

        timer.sleep_and_calc_next_it(iterations_behind);
    }

    broadcast_event(MsgType::RACE_FINISHED);

    handle_upgrades_phase(race_index);
}

void Gameloop::run() {
    broadcast_event(MsgType::GAME_START);
    receive_selected_cars();
    initialize_races();
    std::cout << "Arrancando la carrera" << std::endl;
    for (size_t i = 0; i < races.size(); i++) {
        handle_race(i);
        if (!should_keep_running())
            return;
        std::this_thread::sleep_for(std::chrono::seconds(CLOSED_INTERVAL_TIME));
    }
    broadcast_event(MsgType::GAME_END);
}
