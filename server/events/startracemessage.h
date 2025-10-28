#ifndef START_RACE_MESSAGE_H
#define START_RACE_MESSAGE_H

#include <string>

#include "clienthandlermessage.h"

class StartRaceMessage: public ClientHandlerMessage {
public:
    explicit StartRaceMessage(int client_id):
            ClientHandlerMessage(client_id, MsgType::START_RACE) {}
    virtual ~StartRaceMessage() = default;
};

#endif
