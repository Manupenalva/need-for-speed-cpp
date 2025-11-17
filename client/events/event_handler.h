#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <exception>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../common/cheatcodes.h"
#include "../../common/constants.h"
#include "../../common/messageDTOs.h"
#include "../../common/queue.h"

#include "eventDTO.h"

class EventHandler {
protected:
    Queue<ClientMessageDTO>& message_queue;
    std::unordered_map<SDL_Keycode, uint8_t> key_to_command;

public:
    explicit EventHandler(Queue<ClientMessageDTO>& queue): message_queue(queue) {}
    virtual ~EventHandler() = default;

    virtual bool handle_event(EventDTO& event_dto) = 0;
};

#endif
