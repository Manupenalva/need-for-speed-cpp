#ifndef RECEIVER_H
#define RECEIVER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/messageDTOs.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/thread.h"

class Receiver: public Thread {
private:
    Protocol& protocol;
    Queue<ServerMessageDTO>& server_queue;

    Receiver(const Receiver&) = delete;
    Receiver& operator=(const Receiver&) = delete;

public:
    Receiver(Protocol& protocol, Queue<ServerMessageDTO>& server_queue);

    void run() override;
    void stop() override;
};

#endif
