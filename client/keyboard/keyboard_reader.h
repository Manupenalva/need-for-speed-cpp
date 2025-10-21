#ifndef KEYBOARD_READER_H
#define KEYBOARD_READER_H

#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"
#include "../../common/queue.h"
#include "../events/event_handler.h"
#include "../events/keyDown_handler.h"
#include "../events/keyUp_handler.h"


class KeyboardReader {
private:
    Queue<ClientMessageDTO>& message_queue;
    std::vector<std::unique_ptr<EventHandler>> handlers;

public:
    explicit KeyboardReader(Queue<ClientMessageDTO>& queue);

    void listen_to_keyboard();
};

#endif
