#ifndef CLIENT_HANDLER_MESSAGE_H
#define CLIENT_HANDLER_MESSAGE_H


class ClientHandlerMessage {
public:
    explicit ClientHandlerMessage(int client_id): client_id(client_id) {}
    virtual ~ClientHandlerMessage() = default;

private:
    int client_id;
    
};

#endif