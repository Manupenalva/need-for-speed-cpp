#ifndef RECEIVER_H
#define RECEIVER_H

#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/protocol.h"
#include "command.h"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

class Receiver: public Thread {
    private:
        Protocol& protocol;
        Queue<Command> command_queue;

    public:
        Receiver(Protocol& protocol);
        ~Receiver();

        void run() override;
        void stop() override;

};

#endif