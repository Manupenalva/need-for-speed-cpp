#ifndef SOUNDS_MANAGER_H
#define SOUNDS_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>

enum MusicID { MAIN_MUSIC = 0 };

constexpr const char* MUSIC_PATH = "../client/resources/sounds/Need For Speed Music.mp3"

        enum EffectID {
            CAR_CRASH = 0,
            CAR_BRAKE = 1
        };

const std::map<EffectID, std::string> EFFECTS_PATH = {
        {CAR_CRASH, "../client/resources/sounds/effects/Car Crash Sound.wav"},
        {CAR_BRAKE, "../client/resources/sounds/effects/Car Braking Sound.wav"}};

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
    void play_effect(EffectID id, int loops = 0, float volume);
};

#endif
