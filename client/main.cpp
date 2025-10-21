#include <QApplication>
#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "lobby/lobby.h"

#include "client.h"


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>\n";
            return 1;
        }

        const char* hostname = argv[1];
        const char* servname = argv[2];

        QApplication app(argc, argv);
        // Crear y mostrar la ventana de Lobby (Qt)
        Lobby lobby;
        lobby.show();

        app.exec();

        // Inicializar cliente SDL
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);

        Client client(hostname, servname, 0);  // ID de cliente 0 para esta demo
        client.run();
        client.stop();
        client.join();

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }


    /*
    try {
        QApplication app(argc, argv);
        // Crear y mostrar la ventana de Lobby (Qt)
        Lobby lobby;
        lobby.show();

        app.exec();

        // Inicializar SDL
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);


        // Crear ventana y renderer
        WindowSDL window("Need for Speed - Demo", 800, 600);
        SDL2pp::Renderer& renderer = window.get_renderer();

        // Crear gestor de texturas y cargar recursos
        TextureManager texture_manager(renderer, "../client/assets/");
        texture_manager.load_resources();

        // Crear drawer
        int client_id = 0;
        DrawerSDL drawer(renderer, texture_manager, client_id);

        // Preparar un estado de prueba con un auto
        ServerMessageDTO msg;
        msg.type = MsgType::STATE_UPDATE;
        CarState car;
        car.id = 0;
        car.x = 400.0f;
        car.y = 300.0f;
        car.angle = 270.0f;  // orientación
        car.speed = 0.0f;
        msg.state.cars.push_back(car);


        // Actualizar drawer con el estado y dibujar
        drawer.update_state(msg, 0);

        // Dibujar una vez y mantener la ventana abierta 5s
        window.clear();
        // El drawer en su update_state ya dibuja mapa y autos, pero llamamos explícito
        // para asegurar que se pinte en esta demo
        drawer.update_state(msg, 0);
        window.present();

        SDL_Delay(5000);

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
    */
}
