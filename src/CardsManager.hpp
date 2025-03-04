#ifndef CARDSMANAGER_HPP
#define CARDSMANAGER_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "GameManager.hpp"

class CardsManager
{
    private:
        int nbChoices = 5;
        int imgWidth, imgHeight;
        SDL_Texture *texture;

    public:
        void init();
        void endRound();
    };


#endif