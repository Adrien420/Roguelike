#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "Map.hpp"


class GameManager
{
    private:
        bool isRunning = false;
        int cnt = 0;
	    SDL_Window *window;
        Map map;


    public:
        GameManager(const char* title, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void clean();

        bool getIsRunning() const { return isRunning; };
        void setIsRunning(const bool isRunning_) { isRunning = isRunning_; };

        static SDL_Renderer *renderer;
        static SDL_Event event;

        ~GameManager();
};

#endif