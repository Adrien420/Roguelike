#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Erreur d'initialisation de SDL_mixer: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    clean();
    Mix_CloseAudio();
}

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadSoundEffect(const std::string& id, const std::string& file) {
    Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
    if (!chunk) {
        std::cerr << "Erreur chargement effet sonore " << file << ": " << Mix_GetError() << std::endl;
        return false;
    }
    soundEffects[id] = chunk;
    return true;
}

bool SoundManager::loadMusic(const std::string& id, const std::string& file) {
    Mix_Music* music = Mix_LoadMUS(file.c_str());
    if (!music) {
        std::cerr << "Erreur chargement musique " << file << ": " << Mix_GetError() << std::endl;
        return false;
    }
    musicTracks[id] = music;
    return true;
}

void SoundManager::playSoundEffect(const std::string& id, int loops) {
    if (soundEffects.count(id)) {
        Mix_PlayChannel(-1, soundEffects[id], loops);
    }
}

void SoundManager::playMusic(const std::string& id, int loops) {
    if (musicTracks.count(id)) {
        Mix_PlayMusic(musicTracks[id], loops);
    }
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}

void SoundManager::stopAllSounds() {
    Mix_HaltChannel(-1);
}

void SoundManager::setMusicVolume(int volume) {
    volume = std::max(0, std::min(volume, MIX_MAX_VOLUME)); // Clamp entre 0 et 128
    Mix_VolumeMusic(volume);
}

void SoundManager::setSoundEffectsVolume(int volume) {
    volume = std::max(0, std::min(volume, MIX_MAX_VOLUME)); // Clamp entre 0 et 128
    for (auto& [id, chunk] : soundEffects) {
        Mix_VolumeChunk(chunk, volume);
    }
}

void SoundManager::clean() {
    for (auto& [id, chunk] : soundEffects) {
        Mix_FreeChunk(chunk);
    }
    soundEffects.clear();

    for (auto& [id, music] : musicTracks) {
        Mix_FreeMusic(music);
    }
    musicTracks.clear();
}