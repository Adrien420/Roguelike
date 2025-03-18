#include "AssetManager.hpp"

SDL_Texture* AssetManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(GameManager::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	return tex;
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, AssetManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
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