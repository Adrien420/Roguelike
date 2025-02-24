#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "Map.hpp"
#include "AssetManager.hpp"

class AssetManager;

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
        void clean();

        static SDL_Renderer *renderer;
        static SDL_Event event;
        static bool isRunning;
        static AssetManager* assets;

        ~GameManager();
};

#endif