#include "Map.hpp"
#include <fstream>
#include <iostream>
#include <sstream>  // Pour la mise en forme des noms de fichiers
#include <iomanip>
#include "GameManager.hpp"

Map::Map(std::string path)
{
    LoadTextures();
    LoadMap(path);
}

Map::~Map() {
    for (SDL_Texture* tex : tileTextures) {
        if (tex) {
            SDL_DestroyTexture(tex);
        }
    }
}

void Map::LoadTextures() {
    tileTextures.resize(132, nullptr);  // 132 textures (de 0 à 131)

    for (int i = 0; i <= 131; i++) {
        // Génération du nom du fichier avec un formatage à 4 chiffres
        std::ostringstream oss;
        oss << "../assets/Tiles/tile_" << std::setw(4) << std::setfill('0') << i << ".png";
        std::string fileName = oss.str();

        // Chargement de la texture
        tileTextures[i] = GameManager::assets->LoadTexture(fileName.c_str());

        if (!tileTextures[i]) {
            std::cerr << "Erreur chargement texture : " << fileName << std::endl;
        } else {
            std::cout << "Texture chargée : " << fileName << std::endl;
        }
    }
}

void Map::LoadMap(std::string path) {
    std::ifstream mapFile(path);
    
    if (!mapFile.is_open()) {
        std::cerr << "Erreur : Impossible de charger " << path << std::endl;
        perror("Détails de l'erreur");
        return;
    }

    int value;
    std::vector<int> row;
    int y = 0;

    while (mapFile) {
        row.clear();
        for (int x = 0; x < 1280/tileSize; x++) {
            if (mapFile >> value) {
                row.push_back(value);

                // 🔹 Ajout d’un collider pour certaines tuiles bloquantes (ex: arbres, murs)
                if (value == 130 || value == 131) {  // Choisis les tuiles à bloquer
                    SDL_Rect collider = {x * tileSize, y * tileSize, tileSize, tileSize};
                    colliders.emplace_back(collider);
                }
            }
        }
        if (!row.empty()) {
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

            if (tileType >= 0 && tileType < tileTextures.size() && tileTextures[tileType]) {
                src.x = src.y = 0;
                dest.x = x * tileSize;
                dest.y = y * tileSize;
                SDL_RenderCopy(GameManager::renderer, tileTextures[tileType], NULL, &dest);
            } else {
                std::cerr << "Erreur : Tuile invalide à (" << x << ", " << y << ") : " << tileType << std::endl;
            }
        }
    }
}

// 🔹 Ajout de la fonction pour récupérer les colliders
std::vector<SDL_Rect> Map::getColliders() const {
    return colliders;
}
