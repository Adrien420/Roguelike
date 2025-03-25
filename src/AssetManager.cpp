#include "AssetManager.hpp"

AssetManager::~AssetManager()
{
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
    for (auto& pair : fonts) {
        TTF_CloseFont(pair.second);
    }
    fonts.clear();
}

SDL_Texture* AssetManager::LoadTexture(const char* fileName)
{
    SDL_Surface* tempSurface = IMG_Load(fileName);
    if (!tempSurface) {
        std::cerr << "Erreur : Impossible de charger l'image " << fileName << " : " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(GameManager::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!tex) {
        std::cerr << "Erreur : Impossible de créer la texture pour " << fileName << " : " << SDL_GetError() << std::endl;
    }
    return tex;
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	SDL_Texture* texture = LoadTexture(path);
    if (texture)
    {
        textures.emplace(id, texture);
    }
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
    if (textures.find(id) == textures.end()) {
        std::cerr << "Erreur : texture " << id << " non trouvée !" << std::endl;
        return nullptr;
    }
    return textures[id];
}


void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
    if (!font) {
        std::cerr << "Erreur : Impossible de charger la police " << path << " : " << TTF_GetError() << std::endl;
    } else {
        fonts.emplace(id, font);
    }
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	if (fonts.find(id) == fonts.end())
    {
        std::cerr << "Erreur : police " << id << " non trouvée !" << std::endl;
        return nullptr;
    }
    return fonts[id];
}

SDL_Texture* AssetManager::AddTxt(std::string txt, TTF_Font* font, SDL_Color color, SDL_Rect* destRect, float scale)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, txt.c_str(), color);
    destRect->w = surface->w * scale;
    destRect->h = surface->h * scale;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(GameManager::renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}