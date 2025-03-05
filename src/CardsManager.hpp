#ifndef CARDSMANAGER_HPP
#define CARDSMANAGER_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <map>
#include "GameManager.hpp"

class CardsManager
{
    private:
        int nbChoices = 5;
        int imgWidth, imgHeight, startX, totalWidth, margin;
        SDL_Texture *texture, *textureSelect;
        SDL_Rect destRect, destRectSelect;
        double angle, startAngle, angleSelect;
        bool hasChosen = false, initilized = false;
        std::map<const std::string, std::map<const std::string, const SDL_KeyCode>> playerKeys = {{"player1", {{"Left",SDLK_q}, {"Right",SDLK_d}, {"Select",SDLK_SPACE}}},
		{"player2", {{"Left",SDLK_LEFT}, {"Right",SDLK_RIGHT}, {"Select",SDLK_m}}}};


    public:
        void init();
        void choseCard();
        void changeCard(std::string playerId, int side); //side == -1 si left, 1 si right
        void select(std::string playerId);
    };


#endif