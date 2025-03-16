#pragma once

#include "GameManager.hpp"
#include "Components.hpp"

class IAControllerComponent : public KeyboardController
{
    private:
        Entity *player1;
        TransformComponent *transformPlayer1;
        int currentDirection = -1;
        int timeUntilNextDecision = 200;
        Uint32 decisionStart;
    
    public:
        IAControllerComponent(std::string playerId_, Entity *player1_) : KeyboardController(playerId_), player1(player1_) {}

        void init() override
        {
            KeyboardController::init();
            transformPlayer1 = &player1->getComponent<TransformComponent>();
            srand(time(NULL));
            decisionStart = timeUntilNextDecision;
        }

        int targetPlayerDirection()
        {
            int direction;
            if(abs(transformPlayer1->position.y - transform->position.y) > abs(transformPlayer1->position.x - transform->position.x)) 
            {
                //Distance verticale séparant les 2 joueurs plus importante
                if(transform->position.y > transformPlayer1->position.y)
                    direction = 0; //UP
                else
                    direction = 1; //DOWN
            }
            else
            {
                //Distance horizontale séparant les 2 joueurs plus importante
                if(transform->position.x > transformPlayer1->position.x)
                    direction = 2; //LEFT
                else
                    direction = 3; //RIGHT
            }
            return direction;
        }

        int randomDirection()
        {
            return rand()%4;
        }

        void update() override
        {
            bool earlyReturn = checkPlayerState();
            if(earlyReturn)
				return;

            if(SDL_GetTicks() - decisionStart < timeUntilNextDecision)
                return;

            decisionStart = SDL_GetTicks();
            
            int randomDecision = rand()%100;
            if(randomDecision < 75) // Choix d'une direction ciblant le joueur (humain)
                currentDirection = targetPlayerDirection();
            else // Choix d'une direction aléatoire
                currentDirection = randomDirection();

            switch(currentDirection)
            {
                case 0:
                    applyDirection("Up");
                    break;
                case 1:
                    applyDirection("Down");
                    break;
                case 2:
                    applyDirection("Left");
                    break;
                case 3:
                    applyDirection("Right");
                    break;
                case 404:
                    isSpamming = true;
                    attack();
                    break;
                default:
                    break;
            }
        }
};