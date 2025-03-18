#pragma once

#include <string>
#include <map>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "GameManager.hpp"

class AssetManager
{
    private:
	    std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;

    public:
        AssetManager(){};
        ~AssetManager(){};

        //texture management
        SDL_Texture* LoadTexture(const char* fileName);

        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);

        void AddFont(std::string id, std::string path, int fontSize);
        TTF_Font* GetFont(std::string id);

        SDL_Texture* AddTxt(std::string txt, TTF_Font* font, SDL_Color color, SDL_Rect* destRect, float scale);
};