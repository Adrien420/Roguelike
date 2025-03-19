#ifndef MAP_HPP
#define MAP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Map {
public:
    Map(){};
    Map(std::string path);
    ~Map();

    void LoadMap(std::string path);
    void DrawMap(SDL_Renderer* renderer);
    std::vector<SDL_Rect> getColliders() const;

private:
    SDL_Renderer* renderer;
    std::vector<std::vector<int>> mapData;
    std::vector<SDL_Texture*> tileTextures;
    std::vector<SDL_Rect> colliders;  
    int tileSize = 32;

    void LoadTextures();
};

#endif // MAP_HPP
