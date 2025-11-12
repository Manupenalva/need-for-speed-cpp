#include "game_launcher.h"

GameLauncher::GameLauncher(const char* hostname, const char* servname):
        protocol(servname, hostname) {
    ServerMessageDTO msg = protocol.recv_server_message();
    if (msg.type != MsgType::SEND_CLIENT_ID) {
        throw std::runtime_error("Failed to receive client ID from server");
    }
    id = msg.id;
}

void GameLauncher::launchGame(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // Crear y mostrar la ventana de Lobby (Qt)
    Lobby lobby(protocol);
    lobby.show();

    app.exec();

    // Inicializar SDL
    SDL2pp::SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    Client client(protocol, id);
    client.run();
}
