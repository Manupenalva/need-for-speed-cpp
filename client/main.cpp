#include <QApplication>
#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "lobby/lobby.h"

#include "client.h"


int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>\n";
            return 1;
        }

        const char* hostname = argv[1];
        const char* servname = argv[2];
        const int id = std::stoi(argv[3]);

        QApplication app(argc, argv);
        // Crear y mostrar la ventana de Lobby (Qt)
        Lobby lobby(hostname, servname);
        lobby.show();

        app.exec();

        // Inicializar SDL
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);

        Client client(hostname, servname, id);
        client.run();

        return 0;

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
