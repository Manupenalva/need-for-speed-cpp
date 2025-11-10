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

#include "carBuilder.h"
#include "carPhysics.h"
#include "mapCollisionBuilder.h"
#include "raceBuilder.h"
#define TARGET_FPS 60

Gameloop::Gameloop(
        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
        std::shared_ptr<RaceStruct> race_monitor):
        user_commands_queue(user_commands_queue), race_monitor(race_monitor), races(), frames(0) {}

void Gameloop::update_car_input(const uint16_t& player_id, const uint8_t& action) {
    players_cars[player_id].update_input(action);
}

void Gameloop::broadcast_players(const int& race_index) {
    ServerMessageDTO msg = races[race_index]->get_broadcast_message(frames);

    race_monitor->broadcast(msg);
}

void Gameloop::broadcast_start(const int& race_index) {
    ServerMessageDTO msg;
    msg.type = MsgType::GAME_START;
    race_monitor->broadcast(msg);
    broadcast_players(race_index);
}


void Gameloop::initialize_races() {
    // mando el catalogo a todos los jugadores, luego recibo la respuesta de cada uno a partir de la
    // comand queue. Cuando estén todos recién ahí sigo con la creación de las carreras.

    std::vector<int> players_id = race_monitor->get_player_ids();

    CarBuilder builder("../server/assets/cars_configs/cars_config.yaml");

    for (const auto& id: players_id) {
        uint16_t player_id = id;
        players_cars[player_id] = builder.create_car(player_id, 1);
    }
    // por ahora es un auto con un byte hardcodeado

    // una vez que tengo todos los autos ahí creo todas las carreras recorriendo la carpeta con
    // la libreria correspondiente
    for (const auto& entry: std::filesystem::directory_iterator("../server/assets/race_configs")) {
        if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
            races.push_back(RaceBuilder::create_race(entry.path(), players_cars));
        }
    }
}


void Gameloop::manage_race(const int& race_index) {
    std::cout << "Entre a manage_race (carrera nueva)" << std::endl;
    broadcast_start(race_index);

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
}


void Gameloop::run() {

    initialize_races();
    std::cout << "Tengo para jugar " << races.size() << " carreras" << std::endl;
    for (size_t i = 0; i < races.size(); i++) {
        std::cout << "Estoy usando una carrera" << std::endl;
        manage_race(i);
        if (!should_keep_running())
            return;
    }
}
