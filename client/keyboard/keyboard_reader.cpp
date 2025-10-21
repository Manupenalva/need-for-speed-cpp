#include "keyboard_reader.h"

KeyboardReader::KeyboardReader(Queue<ClientMessageDTO>& queue): message_queue(queue) {
    // Agregar handlers de eventos
    handlers.push_back(std::make_unique<KeyDownHandler>(message_queue));
    handlers.push_back(std::make_unique<KeyUpHandler>(message_queue));
}

void KeyboardReader::listen_to_keyboard() {
    SDL_Event event;
    ClientMessageDTO msg;
    msg.type = MsgType::DRIVING_EVENT;
    while (SDL_PollEvent(&event)) {
        for (auto& handler: handlers) {
            handler->handle_event(event, msg);
        }
    }
    // Si se registraron eventos, enviarlos al queue
    if (!msg.events.empty()) {
        message_queue.push(msg);
    }
}
