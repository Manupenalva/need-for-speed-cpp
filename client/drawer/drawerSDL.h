#ifndef DRAWERSDL_H
#define DRAWERSDL_H

#include "drawer.h"
#include <SDL2pp/SDL2pp.hh>

class DrawerSDL : public Drawer {
    private:
        SDL2pp::Renderer& renderer;

    public:
        explicit DrawerSDL(SDL2pp::Renderer& renderer);
        ~DrawerSDL() override;

        void draw(const ServerMessageDTO& state) override;
};

#endif