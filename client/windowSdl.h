#ifndef WINDOWSDL_H
#define WINDOWSDL_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

class WindowSDL {
    private:
        SDL2pp::Window window;
        SDL2pp::Renderer renderer;

    public:
        WindowSDL(const char* title, int width, int height);
        ~WindowSDL();

        void clear();
        void present();
};

#endif