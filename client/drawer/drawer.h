#ifndef DRAWER_H
#define DRAWER_H

#include "../common/messageDTOs.h"

// Interfaz para dibujadores encargados de pintar el estado en la ventana
class Drawer {
public:
    virtual ~Drawer() = default;

    // Dibujar usando la información del juego (ServerMessageDTO u otros DTOs)
    virtual void draw(const ServerMessageDTO& state) = 0;
};

#endif
