#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

#include <string>
#include <vector>

#include "SocketInterface.h"
#include "carState.h"
#include "checkpointArrow.h"
#include "checkpointInfo.h"
#include "constants.h"
#include "lobbyinfo.h"
#include "messageDTOs.h"
#include "minimapInfo.h"
#include "npcState.h"
#include "socket.h"

class MessageReceiver {
public:
    explicit MessageReceiver(ISocket& socket);
    ClientMessageDTO recv_client_message();
    ServerMessageDTO recv_server_message();

private:
    ISocket& socket;

    State recv_state_update();
    // cppcheck-suppress unusedPrivateFunction
    std::vector<LobbyInfo> recv_lobbies_info();
    std::vector<uint8_t> recv_driving_events();
    bool recv_join_result();

    uint32_t obtain_uint32();
    uint16_t obtain_uint16();
    uint8_t obtain_byte();
    // cppcheck-suppress unusedPrivateFunction
    std::string obtain_string(uint16_t size);
    float uint32_to_float(uint32_t n);
    float obtain_float();
    CarProperties recv_car_properties();
    PlayerState recv_player_state();
    // std::string obtain_lobby_name();

    CarState recv_car_state();
    NpcState recv_npc_state();
    CheckpointInfo recv_checkpoint_info();
    CheckpointArrow recv_checkpoint_arrow();
    LobbyInfo recv_lobby_info();
    std::vector<CarProperties> recv_car_catalog();
    IntervalState recv_interval_state();
    CheatCode recv_cheat_code();
    MinimapInfo recv_minimap_info();
};
#endif
