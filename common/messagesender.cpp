#include "messagesender.h"

#include <algorithm>
#include <cstring>
#include <string>

MessageSender::MessageSender(ISocket& socket): socket(socket), buffer(), offset(0) {}


void MessageSender::send_message(const ServerMessageDTO& msg) {

    switch (msg.type) {
        case MsgType::STATE_UPDATE:
            serialize_state(msg.state);
            break;
        // case MsgType::SEND_LOBBIES_INFO:
        //     serialize_lobbies(msg.lobbies);
        //     break;
        case MsgType::JOIN_RESULT:
            serialize_join_result(msg.joined);
            break;
        case MsgType::SEND_CLIENT_ID:
            serialize_client_id(msg.id);
            break;
        case MsgType::SEND_LOBBY_UPDATE:
            serialize_lobby_update(msg.lobby_info);
            break;
        case MsgType::SEND_CAR_CATALOG:
            serialize_car_catalog(msg.car_catalog);
            break;
        case MsgType::INTERVAL_UPDATE:
            serialize_interval_state(msg.interval_state);
            break;
        case MsgType::SEND_MAP_NUMBER:
            serialize_map_number(msg.map_number);
            break;
        case MsgType::SEND_MINIMAP_INFO:
            serialize_minimap_info(msg.minimap_info);
            break;
        case MsgType::RACE_POSITIONS:
            serialize_race_positions(msg.positions);
            break;
        case MsgType::ACCUMULATED_POSITIONS:
            serialize_accumulated_positions(msg.positions);
            break;
        default:
            buffer.resize(CODE_BYTES);
            offset = 0;
            append_bytes(&msg.type, CODE_BYTES);
            break;
    }
    socket.sendall(buffer.data(), buffer.size());
}


void MessageSender::send_message(const ClientMessageDTO& msg) {

    switch (msg.type) {
        case MsgType::DRIVING_EVENT:
            serialize_events(msg.events);
            break;
        case MsgType::JOIN_RACE:
            serialize_lobby(msg.lobby_id, msg.type);
            break;
        case MsgType::SELECT_CAR:
            serialize_car_number(msg.car_id);
            break;
        case MsgType::CHEAT_CODE:
            serialize_cheat_code(msg.cheat_code);
            break;
        default:
            buffer.resize(CODE_BYTES);
            offset = 0;
            append_bytes(&msg.type, CODE_BYTES);
            break;
    }
    socket.sendall(buffer.data(), buffer.size());
}

void MessageSender::serialize_map_number(const uint8_t map_number) {
    buffer.resize(CODE_BYTES + MAP_NUMBER_BYTES);
    offset = 0;
    MsgType type = MsgType::SEND_MAP_NUMBER;
    append_bytes(&type, CODE_BYTES);
    append_bytes(&map_number, MAP_NUMBER_BYTES);
}

void MessageSender::serialize_race_positions(
        const std::vector<std::pair<uint16_t, float>>& positions) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES + positions.size() * POSITION_BYTES);
    offset = 0;
    MsgType type = MsgType::RACE_POSITIONS;
    append_bytes(&type, CODE_BYTES);
    append_positions(positions);
}

void MessageSender::serialize_accumulated_positions(
        const std::vector<std::pair<uint16_t, float>>& positions) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES + positions.size() * POSITION_BYTES);
    offset = 0;
    MsgType type = MsgType::ACCUMULATED_POSITIONS;
    append_bytes(&type, CODE_BYTES);
    append_positions(positions);
}

void MessageSender::serialize_car_catalog(const std::vector<CarProperties>& catalog) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES + catalog.size() * CAR_PROPERTIES_BYTES);
    offset = 0;
    MsgType type = MsgType::SEND_CAR_CATALOG;
    append_bytes(&type, CODE_BYTES);
    append_uint16(static_cast<uint16_t>(catalog.size()));
    for (const auto& car_prop: catalog) {
        append_car_properties(car_prop);
    }
}

void MessageSender::serialize_interval_state(const IntervalState& interval_state) {
    buffer.resize(2 * AMOUNT_PLAYERS_BYTES + CODE_BYTES + LENGTH_BYTES +
                  interval_state.player_states.size() * PLAYER_STATE_BYTES);
    offset = 0;
    MsgType type = MsgType::INTERVAL_UPDATE;
    append_bytes(&type, CODE_BYTES);
    append_bytes(&interval_state.players_ready, AMOUNT_PLAYERS_BYTES);
    append_bytes(&interval_state.total_players, AMOUNT_PLAYERS_BYTES);
    append_uint16(static_cast<uint16_t>(interval_state.player_states.size()));
    for (const auto& player_state: interval_state.player_states) {
        append_player_state(player_state);
    }
}

void MessageSender::serialize_lobby(const int lobby_id, MsgType type) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES);
    offset = 0;
    append_bytes(&type, CODE_BYTES);
    append_uint16(lobby_id);
}

void MessageSender::serialize_state(const State& state) {
    buffer.resize(CODE_BYTES + COUNTDOWN_BYTES + 2 * LENGTH_BYTES + AMOUNT_BYTES +
                  state.cars.size() * CAR_STATE_BYTES + state.npcs.size() * NPC_STATE_BYTES);
    offset = 0;
    MsgType type = MsgType::STATE_UPDATE;
    append_bytes(&type, CODE_BYTES);
    append_uint16(static_cast<uint16_t>(state.countdown_time));
    append_uint32(state.frame);
    append_uint16(state.cars.size());

    for (const auto& car: state.cars) {
        append_car_state(car);
    }

    append_uint16(static_cast<uint16_t>(state.npcs.size()));
    for (const auto& npc: state.npcs) {
        append_npc_state(npc);
    }
}
void MessageSender::serialize_lobby_update(const LobbyInfo& lobby_info) {
    buffer.resize(LOBBY_BYTES);
    offset = 0;
    MsgType type = MsgType::SEND_LOBBY_UPDATE;
    append_bytes(&type, CODE_BYTES);
    append_uint16(lobby_info.lobby_id);
    append_bytes(&lobby_info.player_amount, AMOUNT_PLAYERS_BYTES);
    append_bytes(&lobby_info.max_players, AMOUNT_PLAYERS_BYTES);
}

void MessageSender::serialize_events(const std::vector<uint8_t>& events) {
    buffer.resize(events.size() + CODE_BYTES + LENGTH_BYTES);
    offset = 0;
    MsgType type = MsgType::DRIVING_EVENT;
    append_bytes(&type, CODE_BYTES);
    append_uint16(events.size());
    append_bytes(events.data(), events.size());
}

void MessageSender::serialize_car_number(const uint16_t car_id) {
    buffer.resize(CODE_BYTES + ID_BYTES);
    offset = 0;
    MsgType type = MsgType::SELECT_CAR;
    append_bytes(&type, CODE_BYTES);
    append_uint16(car_id);
}

void MessageSender::serialize_cheat_code(const CheatCode cheat_code) {
    buffer.resize(CODE_BYTES * 2);
    offset = 0;
    MsgType type = MsgType::CHEAT_CODE;
    append_bytes(&type, CODE_BYTES);
    uint8_t cheat_code_byte = static_cast<uint8_t>(cheat_code);
    append_bytes(&cheat_code_byte, 1);
}

// void MessageSender::serialize_lobbies(const std::vector<LobbyInfo>& lobbies) {

//     offset = 0;
//     size_t total_size = calculate_lobbies_size(lobbies);
//     buffer.resize(total_size);

//     MsgType type = MsgType::SEND_LOBBIES_INFO;
//     append_bytes(&type, CODE_BYTES);
//     append_uint16(lobbies.size());
//     for (const auto& lobby: lobbies) {
//         append_uint16(lobby.name.size());
//         append_bytes(lobby.name.data(), lobby.name.size());
//         append_bytes(&lobby.player_amount, AMOUNT_PLAYERS_BYTES);
//     }
// }

void MessageSender::serialize_join_result(bool joined) {
    buffer.resize(CODE_BYTES + 1);
    offset = 0;
    MsgType type = MsgType::JOIN_RESULT;
    append_bytes(&type, CODE_BYTES);
    uint8_t joined_byte = joined ? 0x01 : 0x00;
    append_bytes(&joined_byte, 1);
}

void MessageSender::serialize_client_id(int id) {
    buffer.resize(CODE_BYTES + ID_BYTES);
    offset = 0;
    MsgType type = MsgType::SEND_CLIENT_ID;
    append_bytes(&type, CODE_BYTES);
    append_uint16(static_cast<uint16_t>(id));
}

void MessageSender::serialize_minimap_info(const MinimapInfo& minimap_info) {
    buffer.resize(CODE_BYTES + 2 * LENGTH_BYTES +
                  minimap_info.checkpoints.size() * CHECKPOINT_INFO_BYTES +
                  minimap_info.arrows.size() * CHECKPOINT_ARROW_BYTES);
    offset = 0;
    MsgType type = MsgType::SEND_MINIMAP_INFO;
    append_bytes(&type, CODE_BYTES);
    append_uint16(static_cast<uint16_t>(minimap_info.checkpoints.size()));
    for (const auto& checkpoint: minimap_info.checkpoints) {
        append_checkpoint_info(checkpoint);
    }
    append_uint16(static_cast<uint16_t>(minimap_info.arrows.size()));
    for (const auto& arrow: minimap_info.arrows) {
        append_checkpoint_arrow(arrow);
    }
}

void MessageSender::append_car_state(const CarState& car) {
    append_uint16(car.id);
    append_float(car.x);
    append_float(car.y);
    append_float(car.angle);
    append_float(car.speed);
    append_uint16(car.lap);
    append_checkpoint_info(car.checkpoint);
    append_checkpoint_arrow(car.checkpoint_arrow);
    uint8_t crashed_byte = car.crashed ? 0x01 : 0x00;
    append_bytes(&crashed_byte, 1);
    uint8_t exploded_byte = car.exploded ? 0x01 : 0x00;
    append_bytes(&exploded_byte, 1);
    uint8_t under_bridge_byte = car.under_bridge ? 0x01 : 0x00;
    append_bytes(&under_bridge_byte, 1);
    uint8_t braking_byte = car.braking ? 0x01 : 0x00;
    append_bytes(&braking_byte, 1);
    append_uint16(car.car_type);
    append_uint16(car.health);
    append_uint16(car.max_health);
}

void MessageSender::append_npc_state(const NpcState& npc) {
    append_float(npc.x);
    append_float(npc.y);
    append_float(npc.angle);
    append_uint16(npc.car_type);
    uint8_t under_bridge_byte = npc.under_bridge ? 0x01 : 0x00;
    append_bytes(&under_bridge_byte, BOOL_BYTES);
}

void MessageSender::append_checkpoint_info(const CheckpointInfo& checkpoint) {
    append_uint16(checkpoint.id);
    append_float(checkpoint.x);
    append_float(checkpoint.y);
    append_float(checkpoint.angle);
    append_float(checkpoint.radius);
    append_bytes(&checkpoint.type, CODE_BYTES);
}

void MessageSender::append_checkpoint_arrow(const CheckpointArrow& arrow) {
    append_float(arrow.x);
    append_float(arrow.y);
    append_float(arrow.angle);
}

void MessageSender::append_car_properties(const CarProperties& car_prop) {
    append_bytes(&car_prop.car_id, 1);
    append_uint16(car_prop.max_speed);
    append_uint16(car_prop.acceleration);
    append_uint16(car_prop.max_health);
    append_uint16(car_prop.mass);
    append_uint16(car_prop.control);
}

void MessageSender::append_player_state(const PlayerState& player_state) {
    append_uint16(player_state.player_id);
    uint8_t ready_byte = player_state.ready ? 0x01 : 0x00;
    append_bytes(&ready_byte, 1);
    append_bytes(&player_state.previous_position, 1);
    append_float(player_state.result_time);
    append_float(player_state.next_penalization_time);
    append_car_properties(player_state.car_properties);
}

void MessageSender::append_positions(const std::vector<std::pair<uint16_t, float>>& positions) {
    append_uint16(static_cast<uint16_t>(positions.size()));
    for (const auto& pos: positions) {
        append_uint16(pos.first);
        append_float(pos.second);
    }
}

void MessageSender::append_bytes(const void* data, size_t size) {
    std::memcpy(buffer.data() + offset, data, size);
    offset += size;
}

uint32_t MessageSender::float_to_uint32(float x) { return static_cast<uint32_t>(x * 100); }

void MessageSender::append_float(float x) {
    uint32_t n = float_to_uint32(x);
    n = htonl(n);
    append_bytes(&n, AMOUNT_BYTES);
}

void MessageSender::append_uint16(uint16_t x) {
    x = htons(x);
    append_bytes(&x, LENGTH_BYTES);
}

void MessageSender::append_uint32(uint32_t x) {
    x = htonl(x);
    append_bytes(&x, AMOUNT_BYTES);
}

// size_t MessageSender::calculate_lobbies_size(const std::vector<LobbyInfo>& lobbies) {
//     size_t total_size = 0;
//     total_size += CODE_BYTES;
//     total_size += LENGTH_BYTES;
//     for (const auto& lobby: lobbies) {
//         total_size += LENGTH_BYTES;
//         total_size += lobby.name.size();
//         total_size += AMOUNT_PLAYERS_BYTES;
//     }
//     return total_size;
// }
