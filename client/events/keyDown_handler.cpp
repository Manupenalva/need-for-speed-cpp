#include "keyDown_handler.h"

KeyDownHandler::KeyDownHandler(Queue<ClientMessageDTO>& queue): EventHandler(queue) {
    // Construir mapa
    key_to_command[SDLK_w] = ACT_ACCEL_PRESS;  // Acelerar
    key_to_command[SDLK_s] = ACT_BRAKE_PRESS;  // Frenar
    key_to_command[SDLK_a] = ACT_LEFT_PRESS;   // Girar izquierda
    key_to_command[SDLK_d] = ACT_RIGHT_PRESS;  // Girar derecha
    key_to_command[SDLK_q] = ACT_NITRO_PRESS;  // Nitro (no se que tecla tiene asignada)

    key_to_command[SDLK_1] = ACT_IMPROVE_SPEED;         // Mejorar velocidad
    key_to_command[SDLK_2] = ACT_IMPROVE_ACCELERATION;  // Mejorar aceleración
    key_to_command[SDLK_3] = ACT_IMPROVE_HEALTH;        // Mejorar salud
    key_to_command[SDLK_4] = ACT_IMPROVE_MASS;          // Mejorar masa
    key_to_command[SDLK_5] = ACT_IMPROVE_HANDLING;      // Mejorar manejo
}

bool KeyDownHandler::handle_event(EventDTO& event_dto) {
    // Manejar el evento de tecla presionada
    if (event_dto.event.type == SDL_KEYDOWN) {
        SDL_Keycode key = event_dto.event.key.keysym.sym;

        // Procesar la tecla presionada según sea necesario
        auto it = key_to_command.find(key);
        if (it != key_to_command.end()) {
            ClientMessageDTO& msg = event_dto.msg;
            uint8_t command = it->second;

            if ((command >= ACT_IMPROVE_SPEED && command <= ACT_IMPROVE_HANDLING &&
                 event_dto.in_race) ||
                (command <= ACT_NITRO_PRESS && !event_dto.in_race)) {
                // Solo permitir mejoras si no está en carrera, y movimientos si está en carrera
                return true;
            }

            msg.events.push_back(command);
        }
    }
    return true;
}
