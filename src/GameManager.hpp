#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "Map.hpp"
#include "AssetManager.hpp"
#include "Entity.hpp"
#include "CardsManager.hpp"
#include "SoundManager.hpp"

class AssetManager;
class CardsManager;
class SoundManager;
class TransformComponent;

class GameManager
{
    private:
        int buttonPlayerWidth = 400, buttonPlayerHeight = 111; //Ratio de 3.6
	    SDL_Window *window;
        Map map;
        SDL_Rect destRectButtonPlayer, destRectButtonIA;

    public:
        GameManager(const char* title, int width, int height, bool fullscreen);

        // Gestion des événements (clavier, souris)
        void handleEvents();
        // Méthode mettant tout à jour pour la prochaine frame
        void update();
        // Méthode permettant le rendu de la nouvelle frame
        void render();
        // Méthode utilisée pour réinitialiser l'état du jeu (positions, projectiles, ...) à la fin d'une manche
        static void reset();
        // Méthode pour libérer la mémoire
        void clean();

        static void createPlayers();
        void homeMenu();
        void initGame();
        // Méthode utilisée pour empêcher le joueur de traverser les obstacles de la map
        void preventMvt(TransformComponent& playerPos, SDL_Rect playerCollider, SDL_Rect obstacleCollider);

        // Variables auxquelles on veut pouvoir accèder / modifier depuis d'autres programmes => static
        static SDL_Renderer *renderer;
        static SDL_Event event;
        // Booléens définissant l'état du jeu
        static bool isRunning, inHomeMenu, chosingCards, inDeathAnimation, isVsIA, modeDEBUG;
        static AssetManager* assets;
        static EntitiesManager entitiesManager;
        static CardsManager cardsManager;
        static SoundManager* soundManager;
        static Entity* player1;
        static Entity* player2;
        // Nombre de manches à remporter pour gagner la partie
        static int nbwinRounds;
        // Nombre de manches remportées par chaque joueur
        static std::map<std::string, int> nbWinsPlayer;

        static void endOfRound(std::string playerId);
        static void startNewRound();

        ~GameManager();
};

#endif // GAMEMANAGER_HPP
