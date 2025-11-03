#include "race.h"

#include <string>

#include "../common/carState.h"
#include "events/actionmessage.h"

#include "hint.h"
#include "mapCollisionBuilder.h"

Race::Race(std::unordered_map<uint16_t, std::unique_ptr<Car>>& players_cars,
           const float& celd_width, const float& celd_height,
           const std::vector<Position>& start_positions, const Position& finish,
           const std::vector<Position>& checkpoints, const std::vector<Hint>& hints,
           const std::string& map_path):
        players_cars(players_cars),
        players_status(),
        celd_width(celd_width),
        celd_height(celd_height),
        start_positions(start_positions),
        checkpoints(checkpoints),
        hints(hints),
        map_collisions_path(map_path),
        current_time(0.0f),
        world() {
    this->checkpoints.push_back(finish);
}

b2WorldId Race::start_race() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    world = b2CreateWorld(&worldDef);

    MapCollisionBuilder::initialize_map_buildings(map_collisions_path, world);

    int i = 0;
    for (const auto& [id, car]: players_cars) {
        car->add_to_world(world, start_positions[i]);
        players_status[id] = {false, 0, 0};
        i++;
    }

    return world;
}

void Race::update_state() {
    for (auto& [id, car]: players_cars) {
        PlayerRaceStatus& status = players_status[id];

        if (status.has_finished)
            continue;

        Position next_checkpoint = checkpoints[status.current_checkpoint_index];
        if (car->reached_checkpoint(next_checkpoint, celd_width, celd_height)) {
            status.current_checkpoint_index++;
            if (status.current_checkpoint_index >= checkpoints.size()) {
                std::cout << "terminé la carrera" << std::endl;
                status.has_finished = true;
            }
        }

        Hint next_hint = hints[status.current_hint_index];
        if (car->reached_checkpoint(next_hint.position, celd_width, celd_height)) {
            if ((status.current_hint_index + 1) < hints.size())
                status.current_hint_index++;
        }

        car->update_physics();
    }

    float timeStep = 1.0f / 60.0f;
    b2World_Step(world, timeStep, 3);

    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished)
            car->update_position();
    }
    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished)
            car->handle_hits();
    }
}

CheckpointInfo Race::get_next_checkpoint_info(const uint16_t car_id) {
    const PlayerRaceStatus& status = players_status[car_id];
    Position checkpoint = checkpoints[status.current_checkpoint_index];

    return {static_cast<uint16_t>(status.current_checkpoint_index),
            static_cast<float>(checkpoint.x), static_cast<float>(checkpoint.x), celd_width};
}

CheckpointArrow Race::get_next_checkpoint_arrow(const uint16_t car_id) {
    const PlayerRaceStatus& status = players_status[car_id];
    Hint hint = hints[status.current_hint_index];

    return {static_cast<float>(hint.position.x), static_cast<float>(hint.position.y), hint.angle};
}

ServerMessageDTO Race::get_broadcast_message(float frames) {
    ServerMessageDTO msg;

    State game_state;
    uint16_t num_cars = 0;
    std::vector<CarState> cars;
    for (const auto& [id, car]: players_cars) {
        CarInfo car_info = car->get_state_info();
        CheckpointInfo checkpoint_info = get_next_checkpoint_info(id);
        CheckpointArrow checkpoint_arrow = get_next_checkpoint_arrow(id);

        CarState state = {
                id,           car_info.x,      car_info.y,       car_info.angle, car_info.speed,
                car_info.lap, checkpoint_info, checkpoint_arrow, false,          1,
                100};

        cars.push_back(state);
        num_cars++;
    }
    game_state.frame = frames;
    game_state.num_cars = num_cars;
    game_state.cars = cars;

    msg.type = MsgType::STATE_UPDATE;
    msg.state = game_state;

    return msg;
}
