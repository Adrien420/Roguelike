#include "TextureManager.hpp"
#include "GameManager.hpp"

std::map<std::string, SDL_Texture*> TextureManager::textures;

SDL_Texture* TextureManager::LoadTexture(const std::string& fileName, SDL_Renderer* renderer) {
    if (textures.find(fileName) != textures.end()) {
        return textures[fileName]; 
    }

    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface) {
        std::cerr << "Erreur IMG_Load : " << fileName << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    
    if (!texture) {
        std::cerr << "Erreur SDL_CreateTextureFromSurface : " << SDL_GetError() << std::endl;
    } else {
        textures[fileName] = texture;
    }

    return texture;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(GameManager::renderer, tex, &src, &dest, 0.0, nullptr, flip);
}

void TextureManager::ClearTextures() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}
