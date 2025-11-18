#include "keyboard_reader.h"

KeyboardReader::KeyboardReader(Queue<ClientMessageDTO>& queue): message_queue(queue) {
    // Agregar handlers de eventos
    handlers.push_back(std::make_unique<KeyDownHandler>(message_queue));
    handlers.push_back(std::make_unique<KeyUpHandler>(message_queue));
    handlers.push_back(std::make_unique<QuitEventHandler>(message_queue));
}

void KeyboardReader::listen_to_keyboard(std::atomic<bool>& running, const bool in_interval,
                                        const bool in_race) {
    SDL_Event event;
    ClientMessageDTO msg;
    while (SDL_PollEvent(&event)) {
        EventDTO event_dto{event, msg, in_interval, in_race, running};
        for (auto& handler: handlers) {
            if (!handler->handle_event(event_dto)) {
                return;
            }
        }
    }
    // Si se registraron eventos, enviarlos al queue
    if (!msg.events.empty() && msg.type != MsgType::CHEAT_CODE) {
        msg.type = MsgType::DRIVING_EVENT;
        message_queue.push(msg);
    }
    if (msg.type == MsgType::CHEAT_CODE) {
        message_queue.push(msg);
    }
}
