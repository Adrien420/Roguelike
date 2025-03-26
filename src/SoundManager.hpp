#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>

// Classe qui gère le chargement, la lecture et l'arrêt des effets sonores et musiques
// On l'implémente en singleton pour qu'une seule instance soit utilisée globalement dans le jeu
class SoundManager {
public:
     // Permet d'accéder à l'unique instance (singleton)
    static SoundManager& getInstance(); 

    bool loadSoundEffect(const std::string& id, const std::string& file, int volume = MIX_MAX_VOLUME);
    bool loadMusic(const std::string& id, const std::string& file, int volume = MIX_MAX_VOLUME);

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