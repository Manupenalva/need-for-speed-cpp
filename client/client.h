#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/gameLoop_timer.h"
#include "../common/messageDTOs.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "config/constants.h"
#include "drawer/drawerSDL.h"
#include "graphics/texture_manager.h"
#include "keyboard/keyboard_reader.h"
#include "sounds_manager/sounds_eventsHandler.h"
#include "window/windowSDL.h"

#include "receiver.h"
#include "sender.h"

class Client: public Thread {
private:
    Protocol& protocol;
    Queue<ClientMessageDTO> events_queue;
    Queue<ServerMessageDTO> server_queue;
    Sender sender;
    Receiver receiver;
    WindowSDL window;
    TextureManager texture_manager;
    SoundsManager sounds_manager;
    SoundsEventsHandler sounds_events_handler;
    DrawerSDL drawer;
    KeyboardReader kb_reader;

    // Último estado recibido del servidor para dibujar
    ServerMessageDTO last_state;
    bool has_last_state;
    bool is_in_race;

    void init_resources();
    void update_state_from_server();
    void clear_display();
    void update_animation_frames(int iterations_ahead);

public:
    Client(Protocol& protocol, const int id);

    void run() override;
    void stop() override;
};

#endif
