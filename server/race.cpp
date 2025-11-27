#include "race.h"

#include <algorithm>
#include <string>
#include <utility>

#include "../common/carState.h"
#include "events/actionmessage.h"

#include "carBuilder.h"
#include "hint.h"
#include "mapCollisionBuilder.h"


Race::Race(std::unordered_map<uint16_t, Car>& players_cars, const float& celd_width,
           const float& celd_height, const std::vector<Position>& start_positions,
           const Position& finish, const std::vector<Position>& checkpoints,
           const std::vector<Hint>& hints, const std::string& map_path, uint8_t city_code,
           float max_time):
        players_cars(players_cars),
        players_status(),
        celd_width(celd_width),
        celd_height(celd_height),
        corners(),
        bridges(),
        start_positions(start_positions),
        checkpoints(checkpoints),
        hints(hints),
        map_collisions_path(map_path),
        current_time(0.0f),
        world(),
        city_code(city_code),
        max_time(max_time) {
    this->checkpoints.push_back(finish);
}

b2WorldId Race::start_race() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    world = b2CreateWorld(&worldDef);

    MapData map_data = MapCollisionBuilder::initialize_map_buildings(map_collisions_path, world);
    bridges = std::move(map_data.bridges);

    for (auto& bridge: bridges) {
        b2Shape_SetUserData(bridge.sensor1_up, &bridge);
        b2Shape_SetUserData(bridge.sensor2_up, &bridge);
        b2Shape_SetUserData(bridge.sensor1_down, &bridge);
        b2Shape_SetUserData(bridge.sensor2_down, &bridge);
    }

    corners = map_data.corners;

    int i = 0;
    for (auto& [id, car]: players_cars) {
        car.add_to_world(world, start_positions[i]);
        players_status[id] = {false, 0, 0};
        i++;
    }

    CarBuilder builder("../server/assets/cars_configs/cars_config.yaml");
    std::transform(map_data.spawn_positions.begin(), map_data.spawn_positions.end(),
                   std::back_inserter(npcs), [&](const Position& spawn_position) {
                       return builder.create_npc(spawn_position, corners, world);
                   });

    return world;
}

void Race::update_state() {
    for (auto& [id, car]: players_cars) {
        PlayerRaceStatus& status = players_status[id];

        if (status.has_finished)
            continue;
        if (car.exploded()) {
            status.has_finished = true;
            float pen_time = car.get_penalization_time();
            race_results.emplace_back(id, max_time + pen_time, pen_time);
            car.finish_race(max_time, MAX_PLAYERS_RACE);
            continue;
        }

        Position next_checkpoint = checkpoints[status.current_checkpoint_index];
        if (car.reached_checkpoint(next_checkpoint, celd_width, celd_height)) {
            status.current_checkpoint_index++;
            if (status.current_checkpoint_index >= checkpoints.size()) {
                status.has_finished = true;
                float pen_time = car.get_penalization_time();
                race_results.emplace_back(id, current_time + pen_time, pen_time);
                car.finish_race(current_time, race_results.size() + 1);
            }
        }
        if (status.current_hint_index < hints.size()) {
            Hint next_hint = hints[status.current_hint_index];
            if (car.reached_checkpoint(next_hint.position, celd_width, celd_height)) {
                if ((status.current_hint_index + 1) < hints.size())
                    status.current_hint_index++;
            }
        }
        car.update_physics();
    }

    for (const auto& npc: npcs) {
        npc->update_position();
    }

    float timeStep = 1.0f / 60.0f;
    b2World_Step(world, timeStep, 3);
    current_time += timeStep;
    if (current_time > max_time) {
        finish_race();
    }

    handle_sensors();

    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished)
            car.update_position();
    }
    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished)
            car.handle_hits();
    }
}

CheckpointInfo Race::get_next_checkpoint_info(const uint16_t car_id) {
    const PlayerRaceStatus& status = players_status[car_id];
    if (status.has_finished) {
        return {0, 0.0f, 0.0f, 0.0f, 0.0f, COMMON_CHECKPOINT};
    }
    Position checkpoint = checkpoints[status.current_checkpoint_index];
    uint8_t type = COMMON_CHECKPOINT;
    if (status.current_checkpoint_index == checkpoints.size() - 1) {
        type = SPECIAL_CHECKPOINT;
    }

    return {static_cast<uint16_t>(status.current_checkpoint_index),
            static_cast<float>(checkpoint.x),
            static_cast<float>(checkpoint.y),
            static_cast<float>(checkpoint.angle),
            celd_width,
            type};
}

CheckpointArrow Race::get_next_checkpoint_arrow(const uint16_t car_id) {
    const PlayerRaceStatus& status = players_status[car_id];
    if (status.has_finished) {
        return {0.0f, 0.0f, 0.0f};
    }
    if (status.current_hint_index < hints.size()) {
        Hint hint = hints[status.current_hint_index];
        return {static_cast<float>(hint.position.x), static_cast<float>(hint.position.y),
                hint.position.angle};
    }
    return {0.0f, 0.0f, 0.0f};
}

ServerMessageDTO Race::get_broadcast_message(float frames) {
    ServerMessageDTO msg;

    State game_state;
    uint16_t num_cars = 0;
    std::vector<CarState> cars;
    for (const auto& [id, car]: players_cars) {
        CarInfo car_info = car.get_state_info();
        CheckpointInfo checkpoint_info = get_next_checkpoint_info(id);
        CheckpointArrow checkpoint_arrow = get_next_checkpoint_arrow(id);

        CarState state(id, car_info.x, car_info.y, car_info.angle, car_info.speed, car_info.lap,
                       checkpoint_info, checkpoint_arrow, car_info.crashed, car_info.exploded,
                       car_info.under_bridge, car_info.braking, car_info.car_type, car_info.health,
                       car_info.max_health);

        cars.push_back(state);
        num_cars++;
    }

    std::vector<NpcState> npcs_state;
    std::transform(npcs.begin(), npcs.end(), std::back_inserter(npcs_state),
                   [](const auto& npc) { return npc->get_state(); });

    game_state.frame = frames;
    game_state.num_cars = num_cars;
    game_state.cars = cars;
    game_state.npcs = npcs_state;
    game_state.remaining_time = max_time - current_time;

    msg.type = MsgType::STATE_UPDATE;
    msg.state = game_state;

    return msg;
}

ServerMessageDTO Race::get_interval_message() {
    ServerMessageDTO msg;

    IntervalState interval_state;
    interval_state.players_ready = 0;
    interval_state.total_players = static_cast<uint8_t>(players_cars.size());
    for (const auto& [id, car]: players_cars) {
        PlayerRaceStatus status = players_status[id];
        interval_state.player_states.push_back(car.get_player_state());
        if (status.has_finished) {
            interval_state.players_ready++;
        }
    }

    msg.type = MsgType::INTERVAL_UPDATE;
    msg.interval_state = interval_state;

    return msg;
}

void Race::finish_race() {
    for (auto& [id, status]: players_status) {
        if (!status.has_finished) {
            float pen_time = players_cars[id].get_penalization_time();
            race_results.emplace_back(id, max_time + pen_time, pen_time);
            players_cars[id].finish_race(max_time, MAX_PLAYERS_RACE);
            status.has_finished = true;
        }
    }
}

bool Race::is_finished() {
    if (current_time >= max_time) {
        return true;
    }

    for (auto& [id, car]: players_cars) {
        if (!players_status[id].has_finished) {
            return false;
        }
    }

    return true;
}

void Race::handle_sensors() {
    b2SensorEvents events = b2World_GetSensorEvents(world);

    for (int i = 0; i < events.beginCount; i++) {
        b2ShapeId sensor_shape = events.beginEvents[i].sensorShapeId;
        const Bridge* bridge = static_cast<Bridge*>(b2Shape_GetUserData(sensor_shape));

        b2ShapeId other_shape = events.beginEvents[i].visitorShapeId;
        b2BodyId other_body = b2Shape_GetBody(other_shape);
        Car* car = static_cast<Car*>(b2Body_GetUserData(other_body));

        handle_bridge_interactions(sensor_shape, bridge, car);
    }
}

void Race::handle_bridge_interactions(b2ShapeId sensor_shape, const Bridge* bridge, Car* car) {
    if (!bridge || !car)
        return;

    if (sensor_shape.index1 == bridge->sensor1_up.index1 ||
        sensor_shape.index1 == bridge->sensor2_up.index1) {
        car->interact_with_bridge(BridgeLayer::TOP);
    } else if (sensor_shape.index1 == bridge->sensor1_down.index1 ||
               sensor_shape.index1 == bridge->sensor2_down.index1) {
        car->interact_with_bridge(BridgeLayer::BOTTOM);
    }
}

void Race::force_finish_race(const uint16_t& player_id) {
    if (players_status[player_id].has_finished)
        return;
    players_status[player_id].has_finished = true;
    float pen_time = players_cars[player_id].get_penalization_time();
    race_results.emplace_back(player_id, current_time + pen_time, pen_time);
    players_cars[player_id].finish_race(current_time, race_results.size() + 1);
}

void Race::force_lose_race(const uint16_t& player_id) {
    Car& car = players_cars[player_id];
    if (players_status[player_id].has_finished)
        return;
    players_status[player_id].has_finished = true;
    float pen_time = car.get_penalization_time();
    race_results.emplace_back(player_id, max_time + pen_time, pen_time);
    car.finish_race(max_time, MAX_PLAYERS_RACE);
    car.explode();
}

uint8_t Race::get_city_code() { return city_code; }

std::vector<CheckpointInfo> Race::get_checkpoints_info() {
    std::vector<CheckpointInfo> checkpoints_info;
    checkpoints_info.reserve(checkpoints.size());
    for (size_t i = 0; i < checkpoints.size(); i++) {
        Position checkpoint = checkpoints[i];
        uint8_t type = COMMON_CHECKPOINT;
        if (i == checkpoints.size() - 1) {
            type = SPECIAL_CHECKPOINT;
        }
        checkpoints_info.push_back({static_cast<uint16_t>(i), static_cast<float>(checkpoint.x),
                                    static_cast<float>(checkpoint.y),
                                    static_cast<float>(checkpoint.angle), celd_width, type});
    }
    return checkpoints_info;
}

std::vector<CheckpointArrow> Race::get_checkpoints_arrows() {
    std::vector<CheckpointArrow> arrows;
    arrows.reserve(hints.size());
    std::transform(hints.begin(), hints.end(), std::back_inserter(arrows), [](const Hint& hint) {
        return CheckpointArrow{static_cast<float>(hint.position.x),
                               static_cast<float>(hint.position.y), hint.position.angle};
    });
    return arrows;
}

const std::vector<std::tuple<uint16_t, float, float>> Race::get_race_results() {
    std::vector<std::tuple<uint16_t, float, float>> results = race_results;
    std::sort(results.begin(), results.end(),
              [](const auto& a, const auto& b) { return std::get<1>(a) < std::get<1>(b); });

    return results;
}

Race::~Race() {
    if (b2World_IsValid(world)) {
        b2DestroyWorld(world);
    }
}
