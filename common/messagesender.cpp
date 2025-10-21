#include "messagesender.h"

#include <algorithm>
#include <cstring>
#include <string>

MessageSender::MessageSender(Socket& socket): socket(socket), buffer(), offset(0) {}


void MessageSender::send_message(const ServerMessageDTO& msg) {

    switch (msg.type) {
        case MsgType::STATE_UPDATE:
            serialize_state(msg.state);
            break;
        case MsgType::SEND_LOBBIES_INFO:
            serialize_lobbies(msg.lobbies);
            break;
        case MsgType::JOIN_RESULT:
            serialize_join_result(msg.joined);
            break;
        case MsgType::SEND_CLIENT_ID:
            serialize_client_id(msg.id);
            break;
        default:
            buffer.resize(CODE_BYTES);
            offset = 0;
            append_bytes(&msg.type, CODE_BYTES);
    }
    socket.sendall(buffer.data(), buffer.size());
}

void MessageSender::send_message(const ClientMessageDTO& msg) {

    switch (msg.type) {
        case MsgType::DRIVING_EVENT:
            serialize_events(msg.events);
            break;
        case MsgType::CREATE_RACE:
        case MsgType::JOIN_RACE:
            serialize_lobby(msg.lobby_name, msg.type);
            break;
        default:
            buffer.resize(CODE_BYTES);
            offset = 0;
            append_bytes(&msg.type, CODE_BYTES);
    }
    socket.sendall(buffer.data(), buffer.size());
}

void MessageSender::serialize_lobby(const std::string& lobby_name, MsgType type) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES + lobby_name.size());
    offset = 0;
    append_bytes(&type, CODE_BYTES);
    append_uint16(lobby_name.size());
    append_bytes(lobby_name.data(), lobby_name.size());
}

void MessageSender::serialize_state(const State& state) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES + AMOUNT_BYTES + state.num_cars * CAR_STATE_BYTES);
    offset = 0;
    MsgType type = MsgType::STATE_UPDATE;
    append_bytes(&type, CODE_BYTES);
    append_uint32(state.frame);
    append_uint16(state.num_cars);

    for (const auto& car: state.cars) {
        append_car_state(car);
    }
}

void MessageSender::serialize_events(const std::vector<uint8_t>& events) {
    buffer.resize(events.size() + CODE_BYTES + LENGTH_BYTES);
    offset = 0;
    MsgType type = MsgType::DRIVING_EVENT;
    append_bytes(&type, CODE_BYTES);
    append_uint16(events.size());
    append_bytes(events.data(), events.size());
}


void MessageSender::serialize_lobbies(const std::vector<LobbyInfo>& lobbies) {

    offset = 0;
    size_t total_size = calculate_lobbies_size(lobbies);
    buffer.resize(total_size);

    MsgType type = MsgType::SEND_LOBBIES_INFO;
    append_bytes(&type, CODE_BYTES);
    append_uint16(lobbies.size());
    for (const auto& lobby: lobbies) {
        append_uint16(lobby.name.size());
        append_bytes(lobby.name.data(), lobby.name.size());
        append_bytes(&lobby.player_amount, AMOUNT_PLAYERS_BYTES);
    }
}

void MessageSender::serialize_join_result(bool joined) {
    buffer.resize(CODE_BYTES + 1);
    offset = 0;
    MsgType type = MsgType::JOIN_RESULT;
    append_bytes(&type, CODE_BYTES);
    uint8_t joined_byte = joined ? 0x01 : 0x00;
    append_bytes(&joined_byte, 1);
}

void MessageSender::serialize_client_id(int id) {
    buffer.resize(CODE_BYTES + AMOUNT_BYTES);
    offset = 0;
    MsgType type = MsgType::SEND_CLIENT_ID;
    append_bytes(&type, CODE_BYTES);
    append_uint32(static_cast<uint32_t>(id));
}

void MessageSender::append_car_state(const CarState& car) {
    append_uint16(car.id);
    append_float(car.x);
    append_float(car.y);
    append_float(car.angle);
    append_float(car.speed);
    append_uint16(car.lap);
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

size_t MessageSender::calculate_lobbies_size(const std::vector<LobbyInfo>& lobbies) {
    size_t total_size = 0;
    total_size += CODE_BYTES;
    total_size += LENGTH_BYTES;
    for (const auto& lobby: lobbies) {
        total_size += LENGTH_BYTES;
        total_size += lobby.name.size();
        total_size += AMOUNT_PLAYERS_BYTES;
    }
    return total_size;
}
