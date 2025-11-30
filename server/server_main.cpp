#include <iostream>

#include "server.h"

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        std::cerr << "Por favor ingrese todos los parametros necesarios" << std::endl;
        return -1;
    }

    std::string servname = argv[1];

    Server server(servname);

    server.start();

    return 0;
}
