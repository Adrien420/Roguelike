#pragma once

#include "GameManager.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include <map>

class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent *sprite;
	// Commentaire temporaire : 
	// Si d est pressé (direction.x = 1), puis q est pressé (direction.x = -1) et relâché (direction = 0)
	// Même si d est toujours pressé, direction.x reste à 0
	// Donc l'idée de ce dictionnaire est de vérifier si la touche pour la direction opposée a bien été relâchée avant de remettre la direction à 0
	std::map<const std::string, bool> isBeingPressed = {{"Up",false}, {"Down",false}, {"Left",false}, {"Right",false}};
	std::map<const std::string, int> velocity = {{"Up",-1}, {"Down",1}, {"Left",-1}, {"Right",1}};


	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void applyDirection(std::string direction)
	{
		if(direction == "Up" || direction == "Down")
		{
			transform->direction.setY(velocity[direction]);
			transform->direction.setX(0);
		}
		else
		{
			transform->direction.setY(0);
			transform->direction.setX(velocity[direction]);
		}
		isBeingPressed[direction] = true;
		sprite->Play("Walk " + direction);
	}

	void changeDirection(std::string direction)
	{
		isBeingPressed[direction] = false;
		for (const auto& pair : isBeingPressed) {
			// Si une autre touche directionnelle est encore pressée : changement de direction (ordre de priorité arbitraire -> déterminé par la map)
			if(pair.first != direction && pair.second)
			{
				applyDirection(pair.first);
				return;
			}
		}
		if(direction == "Up" || direction == "Down")
			transform->direction.setY(0);
		else
			transform->direction.setX(0);
		sprite->Play("Idle " + direction);
	}

	void update() override
	{
		if (GameManager::event.type == SDL_KEYDOWN)
		{
			switch (GameManager::event.key.keysym.sym)
			{
			case SDLK_z:
				applyDirection("Up");
				break;
			case SDLK_q:
				applyDirection("Left");
				break;
			case SDLK_d:
				applyDirection("Right");
				break;
			case SDLK_s:
				applyDirection("Down");
				break;
			default:
				break;
			}
		}
	
		if (GameManager::event.type == SDL_KEYUP)
		{
			switch (GameManager::event.key.keysym.sym)
			{
			case SDLK_z:
				changeDirection("Up");
				break;

			case SDLK_q:
				changeDirection("Left");
				break;

			case SDLK_d:
				changeDirection("Right");
				break;

			case SDLK_s:
				changeDirection("Down");
				break;

			case SDLK_ESCAPE:
				GameManager::isRunning = false;
			default:
				break;
			}
		}
	}
};