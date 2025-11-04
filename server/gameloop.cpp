#include "gameloop.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <ostream>
#include <thread>

#include "../common/gameLoop_timer.h"
#include "../libs/box2d/include/box2d/box2d.h"
#include "events/actionmessage.h"

#include "carPhysics.h"
#include "mapCollisionBuilder.h"
#include "raceBuilder.h"
#define TARGET_FPS 60

Gameloop::Gameloop(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
        std::shared_ptr<RaceStruct> race_monitor):
        user_commands_queue(user_commands_queue),
        race_monitor(race_monitor),
        races(),
        frames(0) {

    std::vector<int> players_id = race_monitor->get_player_ids();

    for (const auto& id: players_id) {
        uint16_t player_id = id;
        players_cars[player_id] = std::make_unique<Car>(player_id);
    }

    races.push_back(RaceBuilder::create_race("../server/assets/race_configs/trial_race.yaml",
                                             players_cars));
}

void Gameloop::update_car_input(const uint16_t& player_id, const uint8_t& action) {
    players_cars[player_id]->update_input(action);
}

void Gameloop::broadcast_players() {
    ServerMessageDTO msg = races[0]->get_broadcast_message(frames);

    race_monitor->broadcast(msg);
}

void Gameloop::broadcast_start() {
    ServerMessageDTO msg;
    msg.type = MsgType::GAME_START;
    race_monitor->broadcast(msg);
    broadcast_players();
}

void Gameloop::run() {

    broadcast_start();
    GameLoopTimer timer(TARGET_FPS);
    uint32_t iterations_behind = 1;
    races[0]->start_race();

    while (should_keep_running()) {
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
            races[0]->update_state();
        }

        broadcast_players();
        frames++;

        timer.sleep_and_calc_next_it(iterations_behind);
    }
}
