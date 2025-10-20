#ifndef RECEIVER
#define RECEIVER

#include <memory>
#include <utility>

#include "../common/constants.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "events/clienthandlermessage.h"

class Receiver: public Thread {
private:
    Protocol& protocol;
    Queue<std::shared_ptr<ClientHandlerMessage>>& queue;
    int id;
    void kill();

public:
    Receiver(Protocol& protocol, Queue<std::shared_ptr<ClientHandlerMessage>>& queue, int id);
    void run() override;
    ~Receiver() override;
};
#endif
