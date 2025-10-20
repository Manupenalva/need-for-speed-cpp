#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

class EventHandler {
public:
    EventHandler() = default;
    virtual ~EventHandler() = default;

    virtual void handle_event() = 0;
};

#endif