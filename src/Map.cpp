#include "Map.hpp"
#include <fstream>
#include <iostream>
#include <sstream>  // Pour la mise en forme des noms de fichiers
#include <iomanip>
#include "GameManager.hpp"

Map::Map(std::string pathMain, std::string pathOverlay)
{
    LoadTextures();
    LoadMap(pathMain, "main");
    LoadMap(pathOverlay, "overlay");
}

Map::~Map() {
    for (SDL_Texture* tex : tileTextures) {
        if (tex) {
            SDL_DestroyTexture(tex);
        }
    }
}

void Map::LoadTextures() {
    //tileTextures.resize(132, nullptr);  // 132 textures (de 0 à 131)

    for (int i = 0; i <= 131; i++) {
        // Génération du nom du fichier avec un formatage à 4 chiffres
        std::ostringstream oss;
        oss << "../assets/Tiles/tile_" << std::setw(4) << std::setfill('0') << i << ".png";
        std::string fileName = oss.str();

        // Chargement de la texture
        GameManager::assets->AddTexture(std::to_string(i), fileName.c_str());

        /*if (!tileTextures[i]) {
            std::cerr << "Erreur chargement texture : " << fileName << std::endl;
        } else {
            std::cout << "Texture chargée : " << fileName << std::endl;
        }*/
    }
}

void Map::LoadMap(std::string path, std::string mapType) {
    std::ifstream mapFile(path);
    
    if (!mapFile.is_open()) {
        std::cerr << "Erreur : Impossible de charger " << path << std::endl;
        perror("Détails de l'erreur");
        return;
    }

    std::vector<int> blockingTiles = {44, 45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 60, 63, 64, 67, 68, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79};

    int value;
    std::vector<int> row;
    int y = 0;

    while (mapFile) {
        row.clear();
        for (int x = 0; x < 1280/tileSize; x++) {
            if (mapFile >> value) {
                row.push_back(value);

                // 🔹 Ajout d’un collider pour certaines tuiles bloquantes (ex: arbres, murs)
                if (std::find(blockingTiles.begin(), blockingTiles.end(), value) != blockingTiles.end()) {  // Choisis les tuiles à bloquer
                    SDL_Rect collider = {x * tileSize, y * tileSize, tileSize, tileSize};
                    colliders.emplace_back(collider);
                }
            }
        }
        if (!row.empty()) {
            if(mapType == "overlay")
                mapOverlayData.push_back(row);
            else
                mapData.push_back(row);
        }
        y++;
    }
    mapFile.close();
}


void Map::DrawMap(SDL_Renderer* renderer) {
    SDL_Rect src, dest;
    src.w = src.h = tileSize;
    dest.w = dest.h = tileSize;

    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++) {
            int tileType = mapData[y][x];
            int tileOverlayType = mapOverlayData[y][x];

            if (tileType >= 0 && tileType <= 131) {
                src.x = src.y = 0;
                dest.x = x * tileSize;
                dest.y = y * tileSize;
                SDL_RenderCopyEx(GameManager::renderer, GameManager::assets->GetTexture(std::to_string(tileType)), NULL, &dest, 0, NULL, SDL_FLIP_NONE);
                SDL_RenderCopyEx(GameManager::renderer, GameManager::assets->GetTexture(std::to_string(tileOverlayType)), NULL, &dest, 0, NULL, SDL_FLIP_NONE);
            } else {
                std::cerr << "Erreur : Tuile invalide à (" << x << ", " << y << ") : " << tileType << std::endl;
            }
        }
    }
    if (GameManager::modeDEBUG)
    {
        for(int i=0; i< colliders.size(); i++)
        {
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 0, 0, 255);  // Mettre le collider en rouge
            SDL_RenderDrawRect(GameManager::renderer, &colliders[i]);
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 255, 255, 255);  // Réinitialiser la couleur du rendu au blanc 
        }
    }
}

// 🔹 Ajout de la fonction pour récupérer les colliders
std::vector<SDL_Rect> Map::getColliders() const {
    return colliders;
}
