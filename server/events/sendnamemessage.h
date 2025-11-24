#ifndef SEND_NAME_MESSAGE_H
#define SEND_NAME_MESSAGE_H

#include <string>

#include "clienthandlermessage.h"

class SendNameMessage: public ClientHandlerMessage {
public:
    SendNameMessage(uint16_t client_id, const std::string& name):
            ClientHandlerMessage(client_id, MsgType::SEND_NAME), name(name) {}
    std::string get_name() const { return name; }

private:
    std::string name;
};

#endif
