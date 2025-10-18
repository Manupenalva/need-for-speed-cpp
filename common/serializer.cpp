#include "serializer.h"

#include <algorithm>
#include <cstring>
#include <string>

Serializer::Serializer(): buffer(), offset(0) {}


std::vector<uint8_t> Serializer::serialize_state(const State& state) {
    buffer.resize(CODE_BYTES + LENGTH_BYTES + AMOUNT_BYTES + state.num_cars * CAR_STATE_BYTES);
    offset = 0;
    MsgType type = MsgType::STATE_UPDATE;
    append_bytes(&type, CODE_BYTES);
    append_uint32(state.frame);
    append_uint16(state.num_cars);

    for (const auto& car: state.cars) {
        append_car_state(car);
    }

    return std::vector<uint8_t>(buffer.begin(), buffer.end());
}

State Serializer::deserialize_state(const std::vector<uint8_t>& data) {
    State state;
    offset = 1;

    state.frame = obtain_uint32(data);

    state.num_cars = obtain_uint16(data);

    state.cars.resize(state.num_cars);

    std::generate(state.cars.begin(), state.cars.end(),
                  [this, &data]() { return parse_car_state(data); });
    return state;
}

std::vector<uint8_t> Serializer::serialize_lobbies(const std::vector<LobbyInfo>& lobbies) {

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
    return std::vector<uint8_t>(buffer.begin(), buffer.end());
}

std::vector<LobbyInfo> Serializer::deserialize_lobbies(const std::vector<uint8_t>& data) {
    offset = 1;
    std::vector<LobbyInfo> lobbies;

    uint16_t size = obtain_uint16(data);
    for (int i = 0; i < size; i++) {
        lobbies.push_back(obtain_lobby(data));
    }
    return lobbies;
}

void Serializer::append_car_state(const CarState& car) {
    append_uint16(car.id);
    append_float(car.x);
    append_float(car.y);
    append_float(car.angle);
    append_float(car.speed);
    append_uint16(car.lap);
}

CarState Serializer::parse_car_state(const std::vector<uint8_t>& data) {
    CarState car;

    car.id = obtain_uint16(data);
    car.x = recover_float(data);
    car.y = recover_float(data);
    car.angle = recover_float(data);
    car.speed = recover_float(data);
    car.lap = obtain_uint16(data);

    return car;
}

float Serializer::recover_float(const std::vector<uint8_t>& data) {
    uint32_t amount = obtain_uint32(data);
    return static_cast<float>(amount) / 100.0f;
}

uint32_t Serializer::obtain_uint32(const std::vector<uint8_t>& data) {
    uint32_t value;
    memcpy(&value, &data[offset], 4);
    offset += 4;
    return ntohl(value);
}

uint16_t Serializer::obtain_uint16(const std::vector<uint8_t>& data) {
    uint16_t value;
    memcpy(&value, &data[offset], 2);
    offset += 2;
    return ntohs(value);
}

uint8_t Serializer::obtain_byte(const std::vector<uint8_t>& data) {
    uint8_t byte;
    memcpy(&byte, &data[offset], 1);
    offset += 1;
    return byte;
}

std::string Serializer::obtain_string(const std::vector<uint8_t>& data, uint16_t size) {
    std::string s;
    s.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        s.push_back(static_cast<char>(data[offset + i]));
    }

    offset += size;
    return s;
}

void Serializer::append_bytes(const void* data, size_t size) {
    std::memcpy(buffer.data() + offset, data, size);
    offset += size;
}

uint32_t Serializer::float_to_uint32(float x) { return static_cast<uint32_t>(x * 100); }

void Serializer::append_float(float x) {
    uint32_t n = float_to_uint32(x);
    n = htonl(n);
    append_bytes(&n, AMOUNT_BYTES);
}

void Serializer::append_uint16(uint16_t x) {
    x = htons(x);
    append_bytes(&x, LENGTH_BYTES);
}

void Serializer::append_uint32(uint32_t x) {
    x = htonl(x);
    append_bytes(&x, AMOUNT_BYTES);
}

size_t Serializer::calculate_lobbies_size(const std::vector<LobbyInfo>& lobbies) {
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

LobbyInfo Serializer::obtain_lobby(const std::vector<uint8_t>& data) {
    LobbyInfo lobby;
    uint16_t name_size = obtain_uint16(data);
    lobby.name = obtain_string(data, name_size);
    lobby.player_amount = obtain_byte(data);
    return lobby;
}
