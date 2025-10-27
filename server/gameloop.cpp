#include "gameloop.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <ostream>
#include <thread>

#include "../libs/box2d/include/box2d/box2d.h"
#include "../libs/box2d/include/box2d/id.h"
#include "events/actionmessage.h"

#include "carPhysics.h"

Gameloop::Gameloop(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
        MonitorGames& games_monitor, int game_id):
        user_commands_queue(user_commands_queue),
        games_monitor(games_monitor),
        game_id(game_id),
        frames(0) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    world = b2CreateWorld(&worldDef);

    std::vector<int> players_id = games_monitor.get_players_id(game_id);

    for (const auto& id: players_id) {
        uint16_t player_id = id;
        players_cars[player_id].state = {
                player_id, 0.0f + player_id * 100, 0.0f + player_id * 100, 0.0f, 0.0f, 0};
        players_cars[player_id] = {
                false,
                false,
                false,
                false,
                players_cars[player_id].state,
                std::make_unique<CarPhysics>(world, players_cars[player_id].state,
                                             0.0f + player_id * 100, 0.0f + player_id * 100)};
    }
}


void Gameloop::update_car_physics(const uint16_t& player_id) {
    CarInputState& car = players_cars[player_id];

    if (car.accelerating) {
        car.physics->accelerate();
    }
    if (car.braking) {
        car.physics->brake();
    }
    if (car.turning_left) {
        car.physics->turn_left();
    }
    if (car.turning_right) {
        car.physics->turn_right();
    }
}


void Gameloop::update_positions() {
    for (auto& [player_id, car]: players_cars) {
        update_car_physics(player_id);
    }

    float timeStep = 1.0f / 60.0f;
    b2World_Step(world, timeStep, 3);

    for (auto& [player_id, car]: players_cars) {
        car.physics->update_position();
    }
    for (auto& [player_id, car]: players_cars) {
        car.physics->handle_hits();
    }
}

// todo lo que sea constantes falla, hay que ponerlos en el .yaml
void Gameloop::update_car_input(const uint16_t& player_id, const uint8_t& action) {
    if (action == ACT_ACCEL_PRESS) {
        players_cars[player_id].accelerating = true;
    } else if (action == ACT_ACCEL_RELEASE) {
        players_cars[player_id].accelerating = false;
    } else if (action == ACT_BRAKE_PRESS) {
        players_cars[player_id].braking = true;
    } else if (action == ACT_BRAKE_RELEASE) {
        players_cars[player_id].braking = false;
    } else if (action == ACT_LEFT_PRESS) {
        players_cars[player_id].turning_left = true;
    } else if (action == ACT_LEFT_RELEASE) {
        players_cars[player_id].turning_left = false;
    } else if (action == ACT_RIGHT_PRESS) {
        players_cars[player_id].turning_right = true;
    } else if (action == ACT_RIGHT_RELEASE) {
        players_cars[player_id].turning_right = false;
    }
}

void Gameloop::broadcast_players() {
    ServerMessageDTO msg;

    State game_state;
    uint16_t num_cars = 0;
    std::vector<CarState> cars;
    for (const auto& [id, car]: players_cars) {
        cars.push_back(car.state);
        num_cars++;
    }
    game_state.frame = frames;
    game_state.num_cars = num_cars;
    game_state.cars = cars;

    msg.type = MsgType::STATE_UPDATE;
    msg.state = game_state;

    games_monitor.broadcast_race_state(game_id, msg);
}

void Gameloop::run() {
    while (should_keep_running()) {
        std::shared_ptr<ClientHandlerMessage> base_msg;
        while (user_commands_queue->try_pop(base_msg)) {
            std::shared_ptr<ActionMessage> msg = std::static_pointer_cast<ActionMessage>(base_msg);
            for (const auto& action: msg->get_actions()) {
                update_car_input(msg->get_client_id(), action);
            }
        }
        update_positions();
        broadcast_players();
        frames++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    games_monitor.remove_race(game_id);
}
