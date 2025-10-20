#ifndef SENDER_H
#define SENDER_H

#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/protocol.h"
#include "../common/messageDTOs.h"

class Sender: public Thread {
    private:
        Protocol& protocol;
        Queue<ClientMessageDTO>& events_queue;

        Sender(const Sender&) = delete;
        Sender& operator=(const Sender&) = delete;

    public:
        Sender(Protocol& protocol, Queue<ClientMessageDTO>& events_queue);

        void run() override;
        void stop() override;

};

#endif