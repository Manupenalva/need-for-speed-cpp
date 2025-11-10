#ifndef SELECT_CAR_MESSAGE_H
#define SELECT_CAR_MESSAGE_H

#include <cstdint>

#include "clienthandlermessage.h"

struct SelectCarMessage: public ClientHandlerMessage {

public:
    SelectCarMessage(uint16_t client_id, uint16_t car_id):
            ClientHandlerMessage(client_id, MsgType::SELECT_CAR), car_id(car_id) {}

    uint16_t get_car_id() const { return car_id; }

private:
    uint16_t car_id;
};

#endif
