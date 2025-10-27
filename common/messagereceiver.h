#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

#include <string>
#include <vector>

#include "carState.h"
#include "constants.h"
#include "lobbyinfo.h"
#include "messageDTOs.h"
#include "socket.h"

class MessageReceiver {
public:
    explicit MessageReceiver(Socket& socket);
    ClientMessageDTO recv_client_message();
    ServerMessageDTO recv_server_message();

private:
    Socket& socket;

    State recv_state_update();
    std::vector<LobbyInfo> recv_lobbies_info();
    std::vector<uint8_t> recv_driving_events();
    bool recv_join_result();

    uint32_t obtain_uint32();
    uint16_t obtain_uint16();
    uint8_t obtain_byte();
    std::string obtain_string(uint16_t size);
    float uint32_to_float(uint32_t n);
    float obtain_float();
    // std::string obtain_lobby_name();

    CarState recv_car_state();
    LobbyInfo recv_lobby_info();
};
#endif
