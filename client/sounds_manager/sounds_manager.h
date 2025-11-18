#ifndef SOUNDS_MANAGER_H
#define SOUNDS_MANAGER_H

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Music.hh>

#include "../config/config_reader.h"
#include "../config/constants.h"

#define CHANNELS 2
#define CHUNK_SIZE 4096

class SoundsManager {
private:
    SDL2pp::Mixer mixer;
    std::unordered_map<MusicID, std::unique_ptr<SDL2pp::Music>> musics;
    std::unordered_map<EffectID, std::unique_ptr<SDL2pp::Chunk>> effects;

public:
    SoundsManager();

    void load_music();
    void play_music(const MusicID id, int loops = -1);
    void stop_music();

    void load_effects();
    void play_effect(EffectID id, int loops = 0, float volume = 1.0f);
};

#endif
