#ifndef ACTION_MESSAGE_H
#define ACTION_MESSAGE_H

#include <vector>
#include "clienthandlermessage.h"
#include <cstdint>

class ActionMessage : public ClientHandlerMessage {
public:
    ActionMessage(int client_id, const std::vector<uint8_t>& actions)
        : ClientHandlerMessage(client_id), actions(actions) {}
    virtual ~ActionMessage() = default;

    std::vector<uint8_t> get_actions() const { return actions; }

private:
    std::vector<uint8_t> actions;
};

#endif
