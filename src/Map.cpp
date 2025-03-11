#include "Map.hpp"
#include <iostream>

Map::Map() 
{
    // Initialisations spécifiques (si besoin)
}

Map::~Map() 
{
    // Libérer les ressources allouées (textures, etc.)
}

bool Map::init(SDL_Window* window) 
{
    // Pour l'instant, nous ne faisons pas grand-chose ici.
    // Plus tard, vous pourrez charger des textures ou lire une map depuis un fichier.
    return true;
}

void Map::render(SDL_Renderer* renderer) 
{
//     // Exemple simple : dessiner un rectangle jaune représentant un objet de décor
//     SDL_Rect rect = { 50, 50, 200, 150 };
//     SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Couleur jaune
//     SDL_RenderFillRect(renderer, &rect);
}
