#ifndef WINDOWSDL_H
#define WINDOWSDL_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../config/config_reader.h"

#include "window.h"

#define DRIVER_RENDERER -1

class WindowSDL: public Window {
private:
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

public:
    explicit WindowSDL(const char* title);
    ~WindowSDL() override;

    void clear() override;
    void present() override;
    SDL2pp::Renderer& get_renderer();
};

#endif
