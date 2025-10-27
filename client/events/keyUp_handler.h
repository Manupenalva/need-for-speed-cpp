#ifndef KEYUP_HANDLER_H
#define KEYUP_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "event_handler.h"

class KeyUpHandler: public EventHandler {
public:
    explicit KeyUpHandler(Queue<ClientMessageDTO>& queue);

    void handle_event(const SDL_Event& event, ClientMessageDTO& msg) override;
};

#endif
