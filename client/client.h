#ifndef CLIENT_H
#define CLIENT_H

#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/protocol.h"
#include "command.h"
#include "windowSdl.h"
#include "sender.h"
#include "receiver.h"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

class Client: public Thread {
    private:    
        Protocol protocol;
        Queue<Command> command_queue;
        Sender sender;
        Receiver receiver;
        WindowSDL window;

        void init_resources();
        void update_state_from_server();
        void clear_display();
        void update_animation_frames();
        void render_frame();
        void sleep_and_calc_next_it();

    public:
        Client(const char* hostname, const char* servname);
        ~Client();

        void run() override;
        void stop() override;

};  

#endif