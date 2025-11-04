#ifndef LOBBY_INFO_H
#define LOBBY_INFO_H
#include <cstdint>
#include <string>


struct LobbyInfo {
    uint16_t lobby_id;
    uint8_t player_amount;
    uint8_t max_players;
};
#endif
