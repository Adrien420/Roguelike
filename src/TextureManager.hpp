#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <map>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const std::string& fileName, SDL_Renderer* renderer);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void ClearTextures(); 

private:
    static std::map<std::string, SDL_Texture*> textures; 
};

#endif // TEXTUREMANAGER_HPP
