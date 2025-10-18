#include "protocol.h"

#include <algorithm>
#include <vector>

Protocol::Protocol(Socket& socket): socket(socket) {}

void Protocol::send_client_message(const ClientMessageDTO& msg) {
    switch (msg.type) {
        case MsgType::DRIVING_EVENT:
            send_driving_events(msg.events);
            break;
        case MsgType::JOIN_RACE:
        case MsgType::CREATE_RACE:
        case MsgType::EXIT_RACE:  // Agrupa todos los de enviar solamente codigo
        case MsgType::GET_LOBBIES:
            send_code(msg.type);
            break;
        default:
            return;
    }
}

void Protocol::send_server_message(const ServerMessageDTO& msg) {
    switch (msg.type) {
        case MsgType::STATE_UPDATE:
            send_state_update(msg.state);
            break;
        case MsgType::GAME_START:
        case MsgType::GAME_END:
            send_code(msg.type);
            break;
        case MsgType::SEND_LOBBIES_INFO:
            send_lobbies(msg.lobbies);
            break;
        case MsgType::JOIN_RESULT:
            send_join_result(msg.joined);
            break;
        default:
            return;
    }
}

void Protocol::send_driving_events(const std::vector<uint8_t>& events) {
    std::vector<uint8_t> message(events.size() + 1);
    message[0] = static_cast<uint8_t>(MsgType::DRIVING_EVENT);
    std::copy(events.begin(), events.end(), message.begin() + CODE_BYTES);
    send_message(message);
}


void Protocol::send_state_update(const State& state_data) {
    send_message(serializer.serialize_state(state_data));
}

State Protocol::recv_state_update(const std::vector<uint8_t>& msg) {
    return serializer.deserialize_state(msg);
}

void Protocol::send_lobbies(const std::vector<LobbyInfo>& lobbies) {
    send_message(serializer.serialize_lobbies(lobbies));
}

void Protocol::send_join_result(bool result) {
    uint8_t result_number = result ? 0x01 : 0x00;
    std::vector<uint8_t> msg({static_cast<uint8_t>(MsgType::JOIN_RESULT), result_number});
    send_message(msg);
}

void Protocol::send_code(MsgType type) { socket.sendall(&type, CODE_BYTES); }

void Protocol::send_message(const std::vector<uint8_t>& msg) {
    socket.sendall(msg.data(), msg.size());
}
