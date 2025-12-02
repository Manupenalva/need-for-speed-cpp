#ifndef GAME_LAUNCHER_H
#define GAME_LAUNCHER_H

#include <QApplication>
#include <stdexcept>

#include "../common/messageDTOs.h"
#include "../common/protocol.h"
#include "lobby/lobby.h"

#include "client.h"

class GameLauncher {
private:
    Protocol protocol;
    uint16_t id;
    bool started = false;

public:
    GameLauncher(const char* hostname, const char* servname);
    void launchGame(int argc, char* argv[]);
};

#endif
