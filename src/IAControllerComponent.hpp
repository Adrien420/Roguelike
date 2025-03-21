#pragma once

#include "GameManager.hpp"
#include "Components.hpp"

class IAControllerComponent : public KeyboardController
{
    private:
        Entity *player1;
        TransformComponent *transformPlayer1;
        int currentDirection = -1;
        std::string currentDirectionLabel = "Down";
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

        bool isTargetInRange(std::string direction)
        {
            bool hasProjectiles = std::get<bool>(stats->stats["hasProjectiles"]);
            if(direction == "Vertical")
            {
                if((abs(transformPlayer1->position.y - transform->position.y) <= 64 || hasProjectiles) && abs(transformPlayer1->position.x - transform->position.x) <= 96)
                    return true;
            }
            else
            {
                if((abs(transformPlayer1->position.x - transform->position.x) <= 64 || hasProjectiles) && abs(transformPlayer1->position.y - transform->position.y) <= 96)
                    return true;
            }
            return false;
        }

        bool attackingDecision()
        {
            bool inRange = false;
            if((currentDirectionLabel == "Up" && (transform->position.y >= transformPlayer1->position.y))
            || (currentDirectionLabel == "Down" && (transform->position.y <= transformPlayer1->position.y)))
                inRange = isTargetInRange("Vertical");
            else if((currentDirectionLabel == "Left" && (transform->position.x >= transformPlayer1->position.x))
            || (currentDirectionLabel == "Right" && (transform->position.x <= transformPlayer1->position.x)))
                inRange = isTargetInRange("Horizontal");
            else
                return false;
            if(!inRange)
                return false;

            // Si le joueur adversaire est à portée d'attaque
            int randomDecision = rand()%100;
            if(randomDecision < 70) // 70% de chances d'attaquer
                return true;
            else
                return false;
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

            bool attacking = attackingDecision();
            if(attacking)
            {
                attack();
                return;
            }
            
            int randomDecision = rand()%100;
            if(randomDecision < 60) // Choix d'une direction ciblant le joueur (humain)
                currentDirection = targetPlayerDirection();
            else if(randomDecision < 90) // Choix d'une direction aléatoire
                currentDirection = randomDirection();
            else //Aucun déplacement
                currentDirection = 4;

            switch(currentDirection)
            {
                case 0:
                    applyDirection("Up");
                    currentDirectionLabel = "Up";
                    break;
                case 1:
                    applyDirection("Down");
                    currentDirectionLabel = "Down";
                    break;
                case 2:
                    applyDirection("Left");
                    currentDirectionLabel = "Left";
                    break;
                case 3:
                    applyDirection("Right");
                    currentDirectionLabel = "Right";
                    break;
                case 4:
                    applyDirection("None");
                    sprite->Play("Idle " + currentDirectionLabel);
                    break;
                default:
                    break;
            }
        }

        void reset() override
        {
            KeyboardController::reset();
            currentDirectionLabel = "Down";
        }
};