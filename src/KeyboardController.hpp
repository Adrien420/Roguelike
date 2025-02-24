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
	std::map<const char*, bool> isBeingPressed = {{"up",false}, {"down",false}, {"left",false}, {"right",false}};


	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (GameManager::event.type == SDL_KEYDOWN)
		{
			switch (GameManager::event.key.keysym.sym)
			{
			case SDLK_z:
				transform->direction.setY(-1);
				isBeingPressed["up"] = true;
				sprite->Play("Walk Up");
				break;
			case SDLK_q:
				transform->direction.setX(-1);
				isBeingPressed["left"] = true;
				sprite->Play("Walk Left");
				break;
			case SDLK_d:
				transform->direction.setX(1);
				isBeingPressed["right"] = true;
				sprite->Play("Walk Right");
				break;
			case SDLK_s:
				transform->direction.setY(1);
				isBeingPressed["down"] = true;
				sprite->Play("Walk Down");
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
				if(!isBeingPressed["down"])
				{
					transform->direction.setY(0);
					sprite->Play("Idle Up");
				}
				else
				{
					transform->direction.setY(-1);
					sprite->Play("Walk Down");
				}
				isBeingPressed["up"] = false;
				break;

			case SDLK_q:
				if(!isBeingPressed["right"])
				{
					transform->direction.setX(0);
					sprite->Play("Idle Left");
				}
				else
				{
					transform->direction.setX(1);
					sprite->Play("Walk Right");
				}
				isBeingPressed["left"] = false;
				break;

			case SDLK_d:
				if(!isBeingPressed["left"])
				{
					transform->direction.setX(0);
					sprite->Play("Idle Right");
				}
				else
				{
					transform->direction.setX(-1);
					sprite->Play("Walk Left");
				}
				isBeingPressed["right"] = false;
				break;

			case SDLK_s:
				if(!isBeingPressed["up"])
				{
					transform->direction.setY(0);
					sprite->Play("Idle Down");
				}
				else
				{
					transform->direction.setY(1);
					sprite->Play("Walk Up");
				}
				isBeingPressed["down"] = false;
				break;

			case SDLK_ESCAPE:
				GameManager::isRunning = false;
			default:
				break;
			}
		}
	}
};