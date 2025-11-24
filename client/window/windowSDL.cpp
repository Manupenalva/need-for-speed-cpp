#include "windowSDL.h"

WindowSDL::WindowSDL(const char* title):
        window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
               ConfigReader::get_instance().get_window_width(),   // Ancho inicial
               ConfigReader::get_instance().get_window_height(),  // Alto inicial
               SDL_WINDOW_RESIZABLE),
        renderer(window, DRIVER_RENDERER, SDL_RENDERER_ACCELERATED) {}

WindowSDL::~WindowSDL() = default;

void WindowSDL::clear() { renderer.Clear(); }

void WindowSDL::present() { renderer.Present(); }

SDL2pp::Renderer& WindowSDL::get_renderer() { return renderer; }
