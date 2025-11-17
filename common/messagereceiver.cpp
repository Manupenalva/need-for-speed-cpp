#include "messagereceiver.h"

#include <algorithm>
#include <cstring>

#include <netinet/in.h>

MessageReceiver::MessageReceiver(ISocket& socket): socket(socket) {}

ClientMessageDTO MessageReceiver::recv_client_message() {
    MsgType type;
    socket.recvall(&type, CODE_BYTES);
    ClientMessageDTO client_msg;
    client_msg.type = type;

    switch (type) {
        case MsgType::DRIVING_EVENT:
            client_msg.events = recv_driving_events();
            break;
        case MsgType::JOIN_RACE:
            client_msg.lobby_id = obtain_uint16();
            break;
        case MsgType::SELECT_CAR:
            client_msg.car_id = obtain_uint16();
            break;
        case MsgType::CHEAT_CODE:
            client_msg.cheat_code = recv_cheat_code();
            break;
        default:
            break;
    }

    return client_msg;
}

ServerMessageDTO MessageReceiver::recv_server_message() {
    MsgType type;
    socket.recvall(&type, CODE_BYTES);
    ServerMessageDTO server_msg;
    server_msg.type = type;

    switch (type) {
        case MsgType::STATE_UPDATE:
            server_msg.state = recv_state_update();
            break;
        // case MsgType::SEND_LOBBIES_INFO:
        //     server_msg.lobbies = recv_lobbies_info();
        // break;
        case MsgType::JOIN_RESULT:
            server_msg.joined = recv_join_result();
            break;
        case MsgType::SEND_CLIENT_ID:
            server_msg.id = static_cast<int>(obtain_uint16());
            break;
        case MsgType::SEND_LOBBY_UPDATE:
            server_msg.lobby_info = recv_lobby_info();
            break;
        case MsgType::SEND_CAR_CATALOG:
            server_msg.car_catalog = recv_car_catalog();
            break;
        case MsgType::INTERVAL_UPDATE:
            server_msg.interval_state = recv_interval_state();
            break;
        case MsgType::SEND_MAP_NUMBER:
            server_msg.map_number = obtain_byte();
            break;
        case MsgType::SEND_MINIMAP_INFO:
            server_msg.minimap_info = recv_minimap_info();
            break;
        default:
            break;
    }

    return server_msg;
}

std::vector<LobbyInfo> MessageReceiver::recv_lobbies_info() {
    uint16_t num_lobbies = obtain_uint16();
    std::vector<LobbyInfo> lobbies(num_lobbies);

    std::generate(lobbies.begin(), lobbies.end(), [this]() { return recv_lobby_info(); });

    return lobbies;
}

MinimapInfo MessageReceiver::recv_minimap_info() {
    MinimapInfo minimap_info;
    uint16_t num_checkpoints = obtain_uint16();
    minimap_info.checkpoints.resize(num_checkpoints);

    std::generate(minimap_info.checkpoints.begin(), minimap_info.checkpoints.end(),
                  [this]() { return recv_checkpoint_info(); });

    uint16_t num_arrows = obtain_uint16();
    minimap_info.arrows.resize(num_arrows);

    std::generate(minimap_info.arrows.begin(), minimap_info.arrows.end(),
                  [this]() { return recv_checkpoint_arrow(); });

    return minimap_info;
}

State MessageReceiver::recv_state_update() {
    State state;
    state.countdown_time = static_cast<int>(obtain_uint16());
    state.frame = obtain_uint32();
    state.num_cars = obtain_uint16();
    state.cars.resize(state.num_cars);

    std::generate(state.cars.begin(), state.cars.end(), [this]() { return recv_car_state(); });

    uint16_t num_npcs = obtain_uint16();
    state.npcs.resize(num_npcs);

    std::generate(state.npcs.begin(), state.npcs.end(), [this]() { return recv_npc_state(); });

    return state;
}

bool MessageReceiver::recv_join_result() {
    uint8_t result = obtain_byte();
    return result != 0;
}

std::vector<uint8_t> MessageReceiver::recv_driving_events() {
    uint16_t events_size = obtain_uint16();
    std::vector<uint8_t> events;
    events.resize(events_size);

    socket.recvall(events.data(), events_size);
    return events;
}

CarState MessageReceiver::recv_car_state() {
    CarState car;
    car.id = obtain_uint16();
    car.x = obtain_float();
    car.y = obtain_float();
    car.angle = obtain_float();
    car.speed = obtain_float();
    car.lap = obtain_uint16();
    car.checkpoint = recv_checkpoint_info();
    car.checkpoint_arrow = recv_checkpoint_arrow();
    uint8_t crashed_byte = obtain_byte();
    car.crashed = (crashed_byte != 0);
    uint8_t exploded_byte = obtain_byte();
    car.exploded = (exploded_byte != 0);
    uint8_t under_bridge_byte = obtain_byte();
    car.under_bridge = (under_bridge_byte != 0);
    uint8_t braking_byte = obtain_byte();
    car.braking = (braking_byte != 0);
    car.car_type = obtain_uint16();
    car.health = obtain_uint16();
    return car;
}

NpcState MessageReceiver::recv_npc_state() {
    NpcState npc;
    npc.x = obtain_float();
    npc.y = obtain_float();
    npc.angle = obtain_float();
    npc.car_type = obtain_uint16();
    uint8_t under_bridge_byte = obtain_byte();
    npc.under_bridge = (under_bridge_byte != 0);
    return npc;
}

CheckpointInfo MessageReceiver::recv_checkpoint_info() {
    CheckpointInfo checkpoint;
    checkpoint.id = obtain_uint16();
    checkpoint.x = obtain_float();
    checkpoint.y = obtain_float();
    checkpoint.angle = obtain_float();
    checkpoint.radius = obtain_float();
    checkpoint.type = obtain_byte();
    return checkpoint;
}

CheckpointArrow MessageReceiver::recv_checkpoint_arrow() {
    CheckpointArrow arrow;
    arrow.x = obtain_float();
    arrow.y = obtain_float();
    arrow.angle = obtain_float();
    return arrow;
}

LobbyInfo MessageReceiver::recv_lobby_info() {
    LobbyInfo lobby;
    lobby.lobby_id = obtain_uint16();
    lobby.player_amount = obtain_byte();
    lobby.max_players = obtain_byte();
    return lobby;
}

IntervalState MessageReceiver::recv_interval_state() {
    IntervalState interval_state;
    interval_state.players_ready = obtain_byte();
    interval_state.total_players = obtain_byte();
    uint16_t num_players = obtain_uint16();
    interval_state.player_states.resize(num_players);

    std::generate(interval_state.player_states.begin(), interval_state.player_states.end(),
                  [this]() { return recv_player_state(); });

    return interval_state;
}

std::vector<CarProperties> MessageReceiver::recv_car_catalog() {
    uint16_t catalog_size = obtain_uint16();
    std::vector<CarProperties> catalog;
    catalog.resize(catalog_size);

    std::generate(catalog.begin(), catalog.end(), [this]() { return recv_car_properties(); });

    return catalog;
}

CarProperties MessageReceiver::recv_car_properties() {
    CarProperties car_prop;
    car_prop.car_id = obtain_byte();
    car_prop.max_speed = obtain_uint16();
    car_prop.acceleration = obtain_uint16();
    car_prop.max_health = obtain_uint16();
    car_prop.mass = obtain_uint16();
    car_prop.control = obtain_uint16();
    return car_prop;
}

PlayerState MessageReceiver::recv_player_state() {
    PlayerState player_state;
    player_state.player_id = obtain_uint16();
    player_state.ready = obtain_byte() != 0;
    player_state.previous_position = obtain_byte();
    player_state.result_time = obtain_uint32();
    player_state.next_penalization_time = obtain_uint32();
    player_state.car_properties = recv_car_properties();
    return player_state;
}

CheatCode MessageReceiver::recv_cheat_code() {
    uint8_t cheat_code_byte = obtain_byte();
    return static_cast<CheatCode>(cheat_code_byte);
}

uint32_t MessageReceiver::obtain_uint32() {
    uint32_t n;
    socket.recvall(&n, AMOUNT_BYTES);
    return ntohl(n);
}

uint16_t MessageReceiver::obtain_uint16() {
    uint16_t n;
    socket.recvall(&n, LENGTH_BYTES);
    return ntohs(n);
}

uint8_t MessageReceiver::obtain_byte() {
    uint8_t byte;
    socket.recvall(&byte, 1);
    return byte;
}

std::string MessageReceiver::obtain_string(uint16_t size) {
    std::string s;
    s.resize(size);
    socket.recvall(s.data(), size);
    return s;
}

float MessageReceiver::uint32_to_float(uint32_t n) { return static_cast<float>(n) / 100.0f; }

float MessageReceiver::obtain_float() {
    uint32_t n = obtain_uint32();
    return uint32_to_float(n);
}

// std::string MessageReceiver::obtain_lobby_name() {
//     uint16_t name_size = obtain_uint16();
//     return obtain_string(name_size);
// }
