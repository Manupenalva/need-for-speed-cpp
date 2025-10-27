#ifndef QUIT_GAME_MESSAGE_H
#define QUIT_GAME_MESSAGE_H

#include "clienthandlermessage.h"

class QuitGameMessage: public ClientHandlerMessage {
public:
    explicit QuitGameMessage(int client_id): ClientHandlerMessage(client_id, MsgType::EXIT_RACE) {}
    virtual ~QuitGameMessage() = default;
};

#endif
