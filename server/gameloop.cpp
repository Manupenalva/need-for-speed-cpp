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
#define TARGET_FPS 60  // esto va a ir con las demas ctes a un .YAML

Gameloop::Gameloop(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
        MonitorClients& games_monitor, int game_id):
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
        players_cars[player_id] = std::make_unique<Car>(player_id, world);
    }
}

void Gameloop::update_positions() {
    for (auto& [player_id, car]: players_cars) {
        car->update_physics();
    }

    float timeStep = 1.0f / 60.0f;
    b2World_Step(world, timeStep, 3);

    for (auto& [player_id, car]: players_cars) {
        car->update_position();
    }
    for (auto& [player_id, car]: players_cars) {
        car->handle_hits();
    }
}

void Gameloop::update_car_input(const uint16_t& player_id, const uint8_t& action) {
    players_cars[player_id]->update_input(action);
}

void Gameloop::broadcast_players() {
    ServerMessageDTO msg;

    State game_state;
    uint16_t num_cars = 0;
    std::vector<CarState> cars;
    for (const auto& [id, car]: players_cars) {
        cars.push_back(car->get_state());
        num_cars++;
    }
    game_state.frame = frames;
    game_state.num_cars = num_cars;
    game_state.cars = cars;

    msg.type = MsgType::STATE_UPDATE;
    msg.state = game_state;

    games_monitor.broadcast_race_state(msg, game_id);
}

void Gameloop::run() {
    GameLoopTimer timer(TARGET_FPS);
    uint32_t iterations_behind = 1;

    while (should_keep_running()) {
        std::shared_ptr<ClientHandlerMessage> base_msg;
        while (user_commands_queue->try_pop(base_msg)) {
            std::shared_ptr<ActionMessage> msg = std::static_pointer_cast<ActionMessage>(base_msg);
            for (const auto& action: msg->get_actions()) {
                update_car_input(msg->get_client_id(), action);
            }
        }

        for (uint32_t i = 0; i < iterations_behind; i++) {
            update_positions();
        }

        broadcast_players();
        frames++;

        timer.sleep_and_calc_next_it(iterations_behind);
    }
}
