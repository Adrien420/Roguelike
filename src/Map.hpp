#ifndef MAP_HPP
#define MAP_HPP
#include <SDL2/SDL.h>

class Map 
{
public:
    Map();
    ~Map();
    
    // Méthode d'initialisation (pour charger des textures, obstacles, etc.)
    bool init(SDL_Window* window);
    
    // Méthode de rendu qui dessine la map sur le renderer
    void render(SDL_Renderer* renderer);
};

#endif // MAP_HPP
