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
	int attackDuration;
	Uint32 attackStart;
	bool isAttacking = false;
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
		attackDuration = sprite->animations["Attack Down"]["frameTime"] * sprite->animations["Attack Down"]["frames"];
	}

	void applyDirection(std::string direction)
	{
		if(direction == "None")
		{
			transform->direction.y = 0;
			transform->direction.x = 0;
			return;
		}

		isBeingPressed[direction] = true;
		if(isAttacking)
			return;
		else if(direction == "Up" || direction == "Down")
		{
			transform->direction.y = velocity[direction];
			transform->direction.x = 0;
		}
		else
		{
			transform->direction.y = 0;
			transform->direction.x = velocity[direction];
		}
		sprite->Play("Walk " + direction);
	}

	void changeDirection(std::string direction, bool keyReleased)
	{
		if(keyReleased)
			isBeingPressed[direction] = false;
		if(isAttacking)
			return;
		for (const auto& pair : isBeingPressed) {
			// Si une autre touche directionnelle est encore pressée : changement de direction (ordre de priorité arbitraire -> déterminé par la map)
			if(pair.second)
			{
				applyDirection(pair.first);
				return;
			}
		}
		// Si aucune autre touche directionnelle n'est pressée
		applyDirection("None");
		sprite->Play("Idle " + direction);
	}

	void attack()
	{
		applyDirection("None");
		int directionIndex = sprite->animIndex%4;
		switch(directionIndex)
		{
			case 0:
				sprite->Play("Attack Down");
				break;
			case 1:
				sprite->Play("Attack Up");
				break;
			case 2:
				sprite->Play("Attack Left");
				break;
			case 3:
				sprite->Play("Attack Right");
				break;
			default:
				break;
		}
		isAttacking = true;
		attackStart = SDL_GetTicks();
	}

	void update() override
	{
		if(isAttacking && (SDL_GetTicks() - attackStart > attackDuration))
		{
			isAttacking = false;
			int directionIndex = sprite->animIndex%4;
			switch(directionIndex)
			{
				case 0:
					changeDirection("Down", false);
					break;
				case 1:
					changeDirection("Up", false);
					break;
				case 2:
					changeDirection("Left", false);
					break;
				case 3:
					changeDirection("Right", false);
					break;
				default:
					break;
			}
		}
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
			case SDLK_SPACE:
				attack();
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
				changeDirection("Up", true);
				break;

			case SDLK_q:
				changeDirection("Left", true);
				break;

			case SDLK_d:
				changeDirection("Right", true);
				break;

			case SDLK_s:
				changeDirection("Down", true);
				break;

			case SDLK_ESCAPE:
				GameManager::isRunning = false;
			default:
				break;
			}
		}
	}
};