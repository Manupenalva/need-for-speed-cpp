#ifndef QUIT_HANDLER_H
#define QUIT_HANDLER_H

#include "event_handler.h"

class QuitEventHandler: public EventHandler {
public:
    explicit QuitEventHandler(Queue<ClientMessageDTO>& queue);

    bool handle_event(EventDTO& event_dto) override;
};

#endif
