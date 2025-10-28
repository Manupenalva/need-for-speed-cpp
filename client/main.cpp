#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "game_launcher.h"


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>\n";
            return 1;
        }

        const char* hostname = argv[1];
        const char* servname = argv[2];

        GameLauncher launcher(hostname, servname);
        launcher.launchGame(argc, argv);

        return 0;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
