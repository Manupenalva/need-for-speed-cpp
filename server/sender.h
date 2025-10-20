#ifndef SENDER
#define SENDER

#include "../common/constants.h"
#include "../common/messageDTOs.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"


class Sender: public Thread {
private:
    Protocol& protocol;
    Queue<ServerMessageDTO> queue;
    void kill();

public:
    explicit Sender(Protocol& protocol);
    void run() override;
    void push_queue(ServerMessageDTO msg);

    ~Sender() override;
};
#endif
