#ifndef SENDER
#define SENDER

#include "../../common/constants.h"
#include "../../common/messageDTOs.h"
#include "../../common/protocol.h"
#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/thread.h"


class Sender: public Thread {
private:
    Protocol& protocol;
    Queue<ServerMessageDTO> queue;
    int id;

    void send_client_id();

public:
    explicit Sender(Protocol& protocol, int id);
    void run() override;
    void push_queue(ServerMessageDTO msg);
    void kill();

    ~Sender() override;
};
#endif
