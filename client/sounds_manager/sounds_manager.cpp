#include "sounds_manager.h"

SoundsManager::SoundsManager():
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNK_SIZE) {}

void SoundsManager::load_music() {
    try {
        musics[MAIN_MUSIC] = std::make_unique<SDL2pp::Music>(MUSIC_PATH);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to load music: ") + MUSIC_PATH +
                                 " Error: " + e.what());
    }
}

void SoundsManager::play_music(const MusicID id, int loops) {
    auto it = musics.find(id);
    if (it != musics.end()) {
        Mix_VolumeMusic(
                ConfigReader::get_instance().get_music_volume());  // Ajustar volumen de la música
        mixer.PlayMusic(*it->second, loops);
    } else {
        throw std::runtime_error("Music ID not found: " + std::to_string(id));
    }
}

void SoundsManager::stop_music() { Mix_HaltMusic(); }

void SoundsManager::load_effects() {
    try {
        auto effects_paths = EFFECTS_PATH;
        for (const auto& [id, path]: effects_paths) {
            effects[id] = std::make_unique<SDL2pp::Chunk>(path);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load sound effect. Error: " + std::string(e.what()));
    }
}

void SoundsManager::play_effect(EffectID id, int loops, float volume) {
    auto it = effects.find(id);
    if (it != effects.end()) {
        int sdl_volume = static_cast<int>(volume * MIX_MAX_VOLUME);
        it->second->SetVolume(sdl_volume);
        mixer.PlayChannel(-1, *it->second, loops);
    } else {
        throw std::runtime_error("Effect ID not found: " + std::to_string(id));
    }
}
