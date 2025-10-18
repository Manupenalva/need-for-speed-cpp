#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

#include <netinet/in.h>

#include "constants.h"
#include "lobbyinfo.h"
#include "msgType.h"
#include "state.h"

class Serializer {
public:
    Serializer();
    ~Serializer() = default;

    std::vector<uint8_t> serialize_state(const State& state);
    State deserialize_state(const std::vector<uint8_t>& data);
    std::vector<uint8_t> serialize_lobbies(const std::vector<LobbyInfo>& lobbies);
    std::vector<LobbyInfo> deserialize_lobbies(const std::vector<uint8_t>& data);

private:
    std::vector<uint8_t> buffer;
    size_t offset;

    void append_car_state(const CarState& car);
    CarState parse_car_state(const std::vector<uint8_t>& data);

    size_t calculate_lobbies_size(const std::vector<LobbyInfo>& lobbies);

    LobbyInfo obtain_lobby(const std::vector<uint8_t>& data);
    float recover_float(const std::vector<uint8_t>& data);
    uint16_t obtain_uint16(const std::vector<uint8_t>& data);
    uint32_t obtain_uint32(const std::vector<uint8_t>& data);
    std::string obtain_string(const std::vector<uint8_t>& data, uint16_t size);
    uint8_t obtain_byte(const std::vector<uint8_t>& data);

    void append_bytes(const void* data, size_t size);
    uint32_t float_to_uint32(float x);
    void append_float(float x);
    void append_uint16(uint16_t x);
    void append_uint32(uint32_t x);
};

#endif
