#include "gameloop.h"

#include <chrono>
#include <cmath>
#include <thread>

#include "events/actionmessage.h"

#define MAX_SPEED 20.0
#define ACCELERATION 5.0
#define ANGLE_ROTATION 4

Gameloop::Gameloop(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
        MonitorClients& games_monitor, int game_id):
        user_commands_queue(user_commands_queue),
        games_monitor(games_monitor),
        game_id(game_id),
        frames(0) {
    std::vector<int> players_id = games_monitor.get_players_id(game_id);

    for (const auto& id: players_id) {
        uint16_t player_id = id;
        players_cars[player_id] = {false, false, false, false, {player_id, 0, 0, 0, 0, 0}};
    }
}


void Gameloop::update_car_state(const uint16_t& player_id) {
    if (players_cars[player_id].accelerating) {
        if (players_cars[player_id].state.speed + ACCELERATION <= MAX_SPEED) {
            players_cars[player_id].state.speed += ACCELERATION;
        }
    }
    if (players_cars[player_id].braking) {
        if (players_cars[player_id].state.speed - ACCELERATION >= 0) {
            players_cars[player_id].state.speed -= ACCELERATION;
        }
    }
    if (players_cars[player_id].turning_left) {
        players_cars[player_id].state.angle -= ANGLE_ROTATION;
    }
    if (players_cars[player_id].turning_right) {
        players_cars[player_id].state.angle += ANGLE_ROTATION;
    }

    if (players_cars[player_id].state.angle < 0.0f) {
        players_cars[player_id].state.angle += 360.0f;
    } else if (players_cars[player_id].state.angle >= 360.f) {
        players_cars[player_id].state.angle -= 360.0f;
    }
}


void Gameloop::update_positions() {
    for (auto& [player_id, car]: players_cars) {

        update_car_state(player_id);

        float rad = car.state.angle * M_PI / 180.0f;
        car.state.x += cosf(rad) * car.state.speed;
        car.state.y += sinf(rad) * car.state.speed;
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

    games_monitor.broadcast_race_state(msg, game_id);
}

void Gameloop::run() {
    while (should_keep_running()) {
        std::shared_ptr<ClientHandlerMessage> base_msg;
        while (user_commands_queue->try_pop(base_msg)) {
            std::shared_ptr<ActionMessage> msg = std::static_pointer_cast<ActionMessage>(base_msg);
            // Estamos haciendo casteo estatico por tener distintos mensajes que heredan de
            // ClientHandlerMessage y tenemos que usar ptr por lo mismo, habría que ver si es
            // realmente necesario
            for (const auto& action: msg->get_actions()) {
                update_car_input(msg->get_client_id(), action);
            }
        }
        update_positions();
        broadcast_players();
        frames++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
