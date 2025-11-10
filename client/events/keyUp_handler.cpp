#include "keyUp_handler.h"


KeyUpHandler::KeyUpHandler(Queue<ClientMessageDTO>& queue): EventHandler(queue) {
    // Construir mapa
    key_to_command[SDLK_w] = ACT_ACCEL_RELEASE;  // Acelerar
    key_to_command[SDLK_s] = ACT_BRAKE_RELEASE;  // Frenar
    key_to_command[SDLK_a] = ACT_LEFT_RELEASE;   // Girar izquierda
    key_to_command[SDLK_d] = ACT_RIGHT_RELEASE;  // Girar derecha
    key_to_command[SDLK_q] = ACT_NITRO_RELEASE;  // Nitro (no se que tecla tiene asignada)
}

bool KeyUpHandler::handle_event(EventDTO& event_dto) {
    // Manejar el evento de tecla presionada
    if (event_dto.event.type == SDL_KEYUP) {
        SDL_Keycode key = event_dto.event.key.keysym.sym;

        // Procesar la tecla presionada según sea necesario
        auto it = key_to_command.find(key);
        if (it != key_to_command.end()) {
            ClientMessageDTO& msg = event_dto.msg;
            uint8_t command = it->second;
            msg.events.push_back(command);
        }
    }
    return true;
}
