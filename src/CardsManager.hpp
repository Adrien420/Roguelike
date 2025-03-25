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
        // Bonus stockés dans le cpp dans des vecteurs de bonus, stockés dans un vecteur => 2 indices nécesssaires pour sélectionner un bonus
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
        // Méthode instanciant tous les bonus (par vecteurs de bonus)
        void initBonus();
        // Méthode choisissant aléatoirement les bonus disponibles à la fin de la manche pour le joueur
        void initBonusPlayer(std::string playerId);
        void init();
        // Méthodes calculant la position (horizontale et verticale) de la première carte en fonction du nombre de choix
        int computeStartX(std::string playerId);
        int computeSelectY(std::string playerId);
        // Méthode affichant les cartes avec les bonus tirés
        void displayPlayerCards(std::string playerId, bool selected);
        // Méthode appelée par le GameManager, gère l'ensemble (initialisation, événements, ...)
        void choseCard();
        // Méthode pour changer de carte sélectionnée
        void changeCard(std::string playerId, SDL_Rect& destRectSelect_, double& angleSelect_, int side); //side == -1 si left, 1 si right
        // Méthode pour sélectionner et appliquer un bonus
        void select(std::string playerId);

        static bool bonusInitialized;
    };


#endif