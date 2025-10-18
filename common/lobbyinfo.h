#ifndef LOBBY_INFO_H
#define LOBBY_INFO_H
#include <cstdint>
#include <string>


struct LobbyInfo {
    std::string name;
    uint8_t player_amount;
};
#endif
