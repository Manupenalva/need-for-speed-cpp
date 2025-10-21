#include "windowSdl.h"

WindowSDL::WindowSDL(const char* title, int width, int height):
        window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
               SDL_WINDOW_SHOWN),
        renderer(window, DRIVER_RENDERER, SDL_RENDERER_ACCELERATED) {
    // Constructor mínimo: crea la ventana y el renderer acelerado.
}

WindowSDL::~WindowSDL() = default;

void WindowSDL::clear() { renderer.Clear(); }

void WindowSDL::present() { renderer.Present(); }

SDL2pp::Renderer& WindowSDL::get_renderer() { return renderer; }
