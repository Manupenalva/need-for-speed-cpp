#include "gameloop.h"

#include <chrono>
#include <cmath>
#include <thread>

Gameloop::Gameloop(Queue<ClientHandlerMessage>& user_commands_queue, std::list<ClientHandler*>& players):
        user_commands_queue(user_commands_queue), players(players), frames(0) {
            for (const auto& clientHandler: players){
                //player_id = clientHandler.get_id();
                players_state[player_id].id = player_id;
                players_state[player_id].x = 0;
                players_state[player_id].y = 0;
                players_state[player_id].speed = 0;
                players_state[player_id].angle = 0;
                players_state[player_id].lap = 0;
            }
        }


void Gameloop::update_positions(){
    for (auto& [player_id, car_state] : players_state) {
        car_state.x += cosf(car_state.angle) * car_state.speed;
        car_state.y += sinf(car_state.angle) * car_state.speed;
    }
}

//todo lo que sea constantes falla, hay que ponerlos en el .yaml
void Gameloop::update_car_stats(const int& player_id, const uint8_t& action) {
    if (action == ACT_ACCEL_PRESS){
        if (players_state[player_id].speed + ACCELERATION <= MAX_SPEED){
            players_state[player_id].speed += ACCELERATION;
        }
    }
    else if (action == ACT_BREAK_PRESS){
        if (players_state[player_id].speed - ACCELERATION >= 0){
            players_state[player_id].speed -= ACCELERATION;
        }
    }
    else if (action == ACT_LEFT_PRESS){
        players_state[player_id].angle -= ANGLE_ROTATION;
    }
    else if (action == ACT_RIGHT_PRESS){
        players_state[player_id].angle += ANGLE_ROTATION;
    }
}

void Gameloop::broadcast_players() {
    State game_state;
    uint16_t num_cars = 0;
    std::vector<CarState> cars;
    for (const auto& [id, car_state]: players_state) {
        cars.push_back(car_state);
        num_cars++;
    }
    game_state.frame = frame;
    game_state.num_cars = num_cars;
    game_state.cars = cars;

    for (const auto& clientHandler: players){
        //enviar datos al cliente desde clientHandler;
    }
}

void Gameloop::run() {
    while (should_keep_running()) {
        ActionMessage msg;
        while (user_commands_queue.try_pop(msg)) {
            //actualizar posicion con el mensaje que haya en la queue y la funcion
            //update_car_stats(mensaje.id, mensaje.accion)
        }
        update_positions();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    client_list.clear();
}
