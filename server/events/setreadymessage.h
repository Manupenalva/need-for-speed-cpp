#ifndef SET_READY_MESSAGE_H
#define SET_READY_MESSAGE_H

#include <cstdint>

#include "clienthandlermessage.h"

struct SetReadyMessage: public ClientHandlerMessage {

public:
    explicit SetReadyMessage(uint16_t client_id):
            ClientHandlerMessage(client_id, MsgType::SET_READY) {}
};

#endif
