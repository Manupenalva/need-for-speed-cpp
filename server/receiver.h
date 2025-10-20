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
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> lobby_queue;
    int id;
    void kill();

public:
    Receiver(Protocol& protocol,
             std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue, int id);
    void run() override;
    ~Receiver() override;
    void set_queue(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue);
};
#endif
