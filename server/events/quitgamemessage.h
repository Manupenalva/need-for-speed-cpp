#ifndef QUIT_GAME_MESSAGE_H
#define QUIT_GAME_MESSAGE_H

#include "clienthandlermessage.h"

class QuitGameMessage: public ClientHandlerMessage {
public:
    explicit QuitGameMessage(int client_id): ClientHandlerMessage(client_id) {}
    virtual ~QuitGameMessage() = default;
};

#endif
