#ifndef KEYUP_HANDLER_H
#define KEYUP_HANDLER_H

#include "event_handler.h"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

class KeyUpHandler : public EventHandler {
public:
    KeyUpHandler(Queue<ClientMessageDTO>& queue) : EventHandler(queue) {
        // Construir mapa
        key_to_command[SDLK_w] = ACT_ACCEL_RELEASE; // Acelerar
        key_to_command[SDLK_s] = ACT_BRAKE_RELEASE; // Frenar
        key_to_command[SDLK_a] = ACT_LEFT_RELEASE; // Girar izquierda
        key_to_command[SDLK_d] = ACT_RIGHT_RELEASE; // Girar derecha
        key_to_command[SDLK_q] = ACT_NITRO_RELEASE; // Nitro (no se que tecla tiene asignada)
    }

    void handle_event(const SDL_Event& event, ClientMessageDTO& msg) override {
        // Manejar el evento de tecla presionada
        if (event.type == SDL_KEYUP) {
            SDL_Keycode key = event.key.keysym.sym;
            
            // Procesar la tecla presionada según sea necesario
            auto it = key_to_command.find(key);
            if (it != key_to_command.end()) {
                uint8_t command = it->second;

                msg.events.push_back(command);
            }

        }
    }
};

#endif