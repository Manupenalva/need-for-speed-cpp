#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <exception>
#include "../../common/messageDTOs.h"
#include "../../common/queue.h"
#include "../../common/constants.h"
#include <unordered_map>

class EventHandler {
protected:
    Queue<ClientMessageDTO>& message_queue;
    std::unordered_map<SDL_Keycode, uint8_t> key_to_command;
    
public:
    EventHandler(Queue<ClientMessageDTO>& queue) : message_queue(queue) {}
    virtual ~EventHandler() = default;

    virtual void handle_event(const SDL_Event& event, ClientMessageDTO& msg) = 0;
};

#endif

