#ifndef SENDER_H
#define SENDER_H

#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/protocol.h"
#include "command.h"


class Sender: public Thread {
    private:
        Protocol& protocol;
        Queue<Command>& command_queue;

    public:
        Sender(Protocol& protocol, Queue<Command>& command_queue);
        ~Sender();

        void run() override;
        void stop() override;

};

#endif