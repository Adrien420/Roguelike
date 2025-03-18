#ifndef CARDSMANAGER_HPP
#define CARDSMANAGER_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <map>
#include "GameManager.hpp"

class CardsManager
{
    private:
        int imgWidth, imgHeight;
        std::map<std::string, int> nbChoices, startX, margin, offsetY, indexSelection;
        std::map<std::string, std::vector<std::array<int, 2>>> selectedBonusIndexes;
        std::map<std::string, std::array<int, 2>> currentBonusIndexes;

        SDL_Texture *texture, *textureSelect;
        std::map<std::string, std::vector<SDL_Texture*>> txtTextures;
        std::map<std::string, std::vector<SDL_Rect>> txtDestRects;
        SDL_Rect destRect, destRectSelect, destRectSelect2;
        double angle, startAngle, angleSelect, angleSelect2;

        std::map<std::string, bool> hasChosen;
        bool initilized = false;
        std::map<const std::string, std::map<const std::string, const SDL_KeyCode>> playerKeys = {{"player1", {{"Left",SDLK_q}, {"Right",SDLK_d}, {"Select",SDLK_SPACE}}},
		{"player2", {{"Left",SDLK_LEFT}, {"Right",SDLK_RIGHT}, {"Select",SDLK_m}}}};

    public:
        void initBonus();
        void initBonusPlayer(std::string playerId);
        void init();
        int computeStartX(std::string playerId);
        int computeSelectY(std::string playerId);
        void displayPlayerCards(std::string playerId);
        void choseCard();
        void changeCard(std::string playerId, SDL_Rect& destRectSelect_, double& angleSelect_, int side); //side == -1 si left, 1 si right
        void select(std::string playerId);

        static bool bonusInitialized;
    };


#endif