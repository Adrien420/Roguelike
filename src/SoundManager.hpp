#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>

class SoundManager {
public:
    static SoundManager& getInstance();  // Singleton pour un accès global

    bool loadSoundEffect(const std::string& id, const std::string& file);
    bool loadMusic(const std::string& id, const std::string& file);

    void playSoundEffect(const std::string& id, int loops = 0);
    void playMusic(const std::string& id, int loops = -1);

    void stopMusic();
    void stopAllSounds();

    void setMusicVolume(int volume);
    void setSoundEffectsVolume(int volume);

    void clean();

private:
    SoundManager();
    ~SoundManager();

    std::map<std::string, Mix_Chunk*> soundEffects;
    std::map<std::string, Mix_Music*> musicTracks;
};

#endif