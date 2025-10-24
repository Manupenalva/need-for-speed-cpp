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
#include "drawer/drawerSDL.h"
#include "graphics/texture_manager.h"
#include "keyboard/keyboard_reader.h"
#include "window/windowSDL.h"

#include "receiver.h"
#include "sender.h"

#define TARGET_FPS 60

#define TITTLE_CLIENT "Need For Speed Race"
#define PATH "../client/assets/"

class Client: public Thread {
private:
    int id;
    Protocol protocol;
    Queue<ClientMessageDTO> events_queue;
    Queue<ServerMessageDTO> server_queue;
    Sender sender;
    Receiver receiver;
    WindowSDL window;
    TextureManager texture_manager;
    DrawerSDL drawer;
    KeyboardReader kb_reader;

    // Último estado recibido del servidor para dibujar
    ServerMessageDTO last_state;
    bool has_last_state;

    void init_resources();
    void update_state_from_server();
    void clear_display();
    void update_animation_frames(int iterations_ahead);

public:
    Client(const char* hostname, const char* servname, const int id);

    void run() override;
    void stop() override;
};

#endif
