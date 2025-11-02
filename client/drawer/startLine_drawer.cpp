#include "startLine_drawer.h"

StartLineDrawer::StartLineDrawer(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
        Drawer(renderer, texture_manager) {}

void StartLineDrawer::draw(const RenderedState& rendered_state) {
    // Todavia falta el mensaje del server que indique donde dibujar la linea de salida
}
