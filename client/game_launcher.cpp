#include "game_launcher.h"

GameLauncher::GameLauncher(const char* hostname, const char* servname):
        protocol(servname, hostname) {}

void GameLauncher::launchGame(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // Crear y mostrar la ventana de Lobby (Qt)
    Lobby lobby(protocol);
    lobby.show();

    app.exec();

    // Inicializar SDL
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);

    Client client(protocol);
    client.run();
}
