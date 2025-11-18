#ifndef KEYDOWN_HANDLER_H
#define KEYDOWN_HANDLER_H

#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "event_handler.h"

class KeyDownHandler: public EventHandler {
private:
    std::unordered_map<SDL_Keycode, CheatCode> key_to_cheat;

public:
    explicit KeyDownHandler(Queue<ClientMessageDTO>& queue);

    bool handle_event(EventDTO& event_dto) override;
};

#endif
