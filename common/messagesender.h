#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <cstdint>
#include <string>
#include <vector>

#include <netinet/in.h>

#include "constants.h"
#include "lobbyinfo.h"
#include "messageDTOs.h"
#include "msgType.h"
#include "socket.h"
#include "state.h"

class MessageSender {
public:
    explicit MessageSender(Socket& socket);
    ~MessageSender() = default;

    void send_message(const ServerMessageDTO& msg);
    void send_message(const ClientMessageDTO& msg);

private:
    Socket& socket;
    std::vector<uint8_t> buffer;
    size_t offset;

    void serialize_state(const State& state);
    void serialize_lobbies(const std::vector<LobbyInfo>& lobbies);
    void serialize_events(const std::vector<uint8_t>& events);
    void serialize_join_result(bool joined);
    void serialize_lobby(const int lobby_id, MsgType type);
    void serialize_client_id(int id);
    void serialize_lobby_update(const LobbyInfo& lobby_info);

    void append_car_state(const CarState& car);
    void append_npc_state(const NpcState& npc);
    void append_checkpoint_info(const CheckpointInfo& checkpoint);
    void append_checkpoint_arrow(const CheckpointArrow& arrow);

    size_t calculate_lobbies_size(const std::vector<LobbyInfo>& lobbies);

    void append_bytes(const void* data, size_t size);
    uint32_t float_to_uint32(float x);
    void append_float(float x);
    void append_uint16(uint16_t x);
    void append_uint32(uint32_t x);
};

#endif
