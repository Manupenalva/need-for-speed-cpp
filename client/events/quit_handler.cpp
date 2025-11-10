#include "quit_handler.h"

QuitEventHandler::QuitEventHandler(Queue<ClientMessageDTO>& queue): EventHandler(queue) {}

bool QuitEventHandler::handle_event(EventDTO& event_dto) {
    if (event_dto.event.type == SDL_QUIT) {
        ClientMessageDTO& msg = event_dto.msg;
        event_dto.keep_running = false;
        msg.type = MsgType::END_CONNECTION;
        message_queue.push(msg);
        return false;
    }
    return true;
}
