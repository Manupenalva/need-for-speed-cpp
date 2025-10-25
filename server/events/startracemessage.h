#ifndef START_RACE_MESSAGE_H
#define START_RACE_MESSAGE_H

#include <string>

#include "clienthandlermessage.h"

class StartRaceMessage: public ClientHandlerMessage {
public:
    StartRaceMessage(int client_id, int race_id):
            ClientHandlerMessage(client_id, MsgType::START_RACE), race_id(race_id) {}
    virtual ~StartRaceMessage() = default;
    
    int get_race_id() const override {
        return race_id;
    }

private:
    int race_id;
};

#endif
