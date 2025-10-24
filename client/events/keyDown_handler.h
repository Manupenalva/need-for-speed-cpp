#ifndef KEYDOWN_HANDLER_H
#define KEYDOWN_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "event_handler.h"

class KeyDownHandler: public EventHandler {
public:
    explicit KeyDownHandler(Queue<ClientMessageDTO>& queue);

    void handle_event(const SDL_Event& event, ClientMessageDTO& msg) override;
};

#endif
