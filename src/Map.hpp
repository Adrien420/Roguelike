#ifndef MAP_HPP
#define MAP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "TextureManager.hpp"
#include "ColliderComponent.hpp"

class Map {
public:
    Map(std::string path, SDL_Renderer* renderer);
    ~Map();

    void LoadMap(std::string path);
    void DrawMap(SDL_Renderer* renderer);
    std::vector<ColliderComponent> getColliders() const;

private:
    SDL_Renderer* renderer;
    std::vector<std::vector<int>> mapData;
    std::vector<SDL_Texture*> tileTextures;
    std::vector<ColliderComponent> colliders;  
    int tileSize = 32;

    void LoadTextures();
};

#endif // MAP_HPP
