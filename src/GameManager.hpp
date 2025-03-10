#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "Map.hpp"
#include "AssetManager.hpp"
#include "Entity.hpp"
#include "CardsManager.hpp"

class AssetManager;
class CardsManager;

class GameManager
{
    private:
        int cnt = 0;
	    SDL_Window *window;
        Map map;

    public:
        GameManager(const char* title, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void reset();
        void clean();

        void pause(bool isPausing_);

        static SDL_Renderer *renderer;
        static SDL_Event event;
        static bool isRunning, isPausing, chosingCards;
        static AssetManager* assets;
        static EntitiesManager entitiesManager;
        static CardsManager cardsManager;
        static Entity* player1;
        static Entity* player2;

        ~GameManager();
};

#endif