#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <atomic>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../common/messageDTOs.h"

struct EventDTO {
    const SDL_Event event;
    ClientMessageDTO& msg;
    const bool in_interval;
    const bool in_race;
    std::atomic<bool>& keep_running;
};

#endif
