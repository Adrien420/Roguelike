#pragma once

#include <string>
#include <map>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "GameManager.hpp"

// Classe qui gère le chargement et le stockage des ressources du jeu (textures, polices, textes...)
class AssetManager
{
    private:
        // Stockage des textures & polices d'écriture par identifiant
	    std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;

    public:
        AssetManager(){};
        ~AssetManager();

         // Permet de charger une texture à partir d’un fichier
        SDL_Texture* LoadTexture(const char* fileName);

          // Ajoute une texture au gestionnaire à partir d’un chemin, avec un identifiant associé
        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);

        void AddFont(std::string id, std::string path, int fontSize);
        TTF_Font* GetFont(std::string id);

        // Permet de créer une texture à partir d’un texte, d’une police et d’une couleur,
        // On met à jour la taille du rectangle passé en paramètre avec l’échelle souhaitée
        SDL_Texture* AddTxt(std::string txt, TTF_Font* font, SDL_Color color, SDL_Rect* destRect, float scale);
};