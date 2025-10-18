#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <vector>

#include <netinet/in.h>

#include "constants.h"
#include "messageDTOs.h"
#include "msgType.h"
#include "serializer.h"
#include "socket.h"
#include "state.h"
class Protocol {
public:
    explicit Protocol(Socket& socket);
    ~Protocol() = default;

    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    void send_client_message(const ClientMessageDTO& msg);
    void send_server_message(const ServerMessageDTO& msg);

    ClientMessageDTO recv_client_message();
    ServerMessageDTO recv_server_message();

    void send_driving_events(const std::vector<uint8_t>& events);
    void send_join_race();
    void send_create_race();

    void send_game_start();
    void send_game_end();
    void send_state_update(const State& state_data);

    State recv_state_update(const std::vector<uint8_t>& msg);

private:
    Socket& socket;
    Serializer serializer;

    void send_message(const std::vector<uint8_t>& msg);
    std::vector<uint8_t> recv_message(MsgType& type);
    void send_code(MsgType type);

    void send_join_result(bool result);
    void send_lobbies(const std::vector<LobbyInfo>& lobbies);
};

#endif
