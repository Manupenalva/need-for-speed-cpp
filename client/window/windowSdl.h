#ifndef WINDOWSDL_H
#define WINDOWSDL_H

#include "window.h"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

#define DRIVER_RENDERER -1

class WindowSDL : public Window {
    private:
        SDL2pp::Window window;
        SDL2pp::Renderer renderer;

    public:
        WindowSDL(const char* title, int width, int height);
        ~WindowSDL() override;

        void clear() override;
        void present() override;
        SDL2pp::Renderer& get_renderer();
};

#endif