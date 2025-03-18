#include <SDL2/SDL.h>
#include "Map.hpp"

int main(int argc, char* argv[]) {
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur d'initialisation SDL : %s", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Test Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Erreur de création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur de création du renderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Charger la map
    Map testMap("../assets/map.txt", renderer);

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // Affichage de la map
        SDL_RenderClear(renderer);
        testMap.DrawMap(renderer);  // Correction : on passe le renderer
        SDL_RenderPresent(renderer);
    }

    // Nettoyage SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
