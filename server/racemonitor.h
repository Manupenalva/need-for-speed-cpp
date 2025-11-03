#ifndef RACEMONITOR_H
#define RACEMONITOR_H

#include <memory>
#include <mutex>
#include <vector>

#include "client_handler.h"

struct RaceStruct {
    std::vector<std::shared_ptr<ClientHandler>> clients;
    std::mutex mtx;
};

#endif
