#ifndef CLIENT_HANDLER_MESSAGE_H
#define CLIENT_HANDLER_MESSAGE_H


#include "../../common/msgType.h"
class ClientHandlerMessage {
public:
    explicit ClientHandlerMessage(int client_id, MsgType msg_type):
            client_id(client_id), msg_type(msg_type) {}
    int get_client_id() const { return client_id; }
    MsgType get_msg_type() const { return msg_type; }
    virtual ~ClientHandlerMessage() = default;
    virtual int get_race_id() { return -1; }

private:
    int client_id;
    MsgType msg_type;
};

#endif
