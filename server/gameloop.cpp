#include "gameloop.h"

#include <chrono>
#include <cmath>
#include <thread>

Gameloop::Gameloop(const std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>>& user_commands_queue, MonitorGames& games_monitor, const int& id):
        user_commands_queue(user_commands_queue), games_monitor(games_monitor), game_id(id), frames(0) {
            std::vector<int> players_id = games_monitor.get_players_id(game_id);

            for (const auto& player_id: players_id){
                players_cars[player_id] = {
                    .accelerating = false, .breaking = false,
                    .turning_right = false, .turning_left = false,
                    .state = {
                        .id = player_id, .x = 0, .y = 0,
                        .speed = 0, .angle = 0, .lap = 0
                    }
                };
            }
        }


void Gameloop::update_car_state(const uint16_t& player_id){
    if (players_cars[player_id].accelerating){
        if (players_cars[player_id].state.speed + ACCELERATION <= MAX_SPEED){
            players_cars[player_id].state.speed += ACCELERATION;
        }
    }
    if (players_cars[player_id].breaking){
        if (players_cars[player_id].state.speed - ACCELERATION >= 0){
            players_cars[player_id].state.speed -= ACCELERATION;
        }
    }
    if (players_cars[player_id].turning_left){
        players_cars[player_id].state.angle -= ANGLE_ROTATION;
    }
    if (players_cars[player_id].turning_right){
        players_cars[player_id].state.angle += ANGLE_ROTATION;
    }
}


void Gameloop::update_positions(){
    for (auto& [player_id, car] : players_cars) {

        update_car_state(player_id);

        car.state.x += cosf(car.state.angle) * car.state.speed;
        car.state.y += sinf(car.state.angle) * car.state.speed;
    }
}

//todo lo que sea constantes falla, hay que ponerlos en el .yaml
void Gameloop::update_car_input(const uint16_t& player_id, const uint8_t& action) {
    if (action == ACT_ACCEL_PRESS){
        players_cars[player_id].accelerating = true;
    }
    else if (action == ACT_ACCEL_RELEASE){
        players_cars[player_id].accelerating = false;
    }
    else if (action == ACT_BREAK_PRESS){
        players_cars[player_id].breaking = true;
    }
    else if (action == ACT_BREAK_RELEASE){
        players_cars[player_id].breaking = false;
    }
    else if (action == ACT_LEFT_PRESS){
        players_cars[player_id].turning_left = true;
    }
    else if (action == ACT_LEFT_RELEASE){
        players_cars[player_id].turning_left = false;
    }
    else if (action == ACT_RIGHT_PRESS){
        players_cars[player_id].turning_right = true;
    }
    else if (action == ACT_RIGHT_RELEASE){
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
            //Estamos haciendo casteo estatico por tener distintos mensajes que heredan de ClientHandlerMessage
            //y tenemos que usar ptr por lo mismo, habría que ver si es realmente necesario
            for (const auto& action: msg->get_actions()){
                update_car_input(msg->get_id(), action);
            }
        }
        update_positions();
        frames++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    games_monitor.remove_race(game_id);
}
