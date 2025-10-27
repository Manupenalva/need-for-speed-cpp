#include "messagereceiver.h"

#include <algorithm>
#include <cstring>

#include <netinet/in.h>

MessageReceiver::MessageReceiver(Socket& socket): socket(socket) {}

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
        case MsgType::SEND_LOBBIES_INFO:
            server_msg.lobbies = recv_lobbies_info();
            break;
        case MsgType::JOIN_RESULT:
            server_msg.joined = recv_join_result();
            break;
        case MsgType::SEND_CLIENT_ID:
            server_msg.id = static_cast<int>(obtain_uint32());
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


State MessageReceiver::recv_state_update() {
    State state;
    state.frame = obtain_uint32();
    state.num_cars = obtain_uint16();
    state.cars.resize(state.num_cars);

    std::generate(state.cars.begin(), state.cars.end(), [this]() { return recv_car_state(); });

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
    return car;
}

LobbyInfo MessageReceiver::recv_lobby_info() {
    LobbyInfo lobby;
    uint16_t name_size = obtain_uint16();
    lobby.name = obtain_string(name_size);
    lobby.player_amount = obtain_byte();
    return lobby;
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
