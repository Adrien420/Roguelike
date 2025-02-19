#pragma once

#include "GameManager.hpp"
#include "Entity.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

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
				sprite->Play("Walk Up");
				break;
			case SDLK_q:
				transform->direction.setX(-1);
				sprite->Play("Walk Left");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_d:
				transform->direction.setX(1);
				sprite->Play("Walk Right");
				break;
			case SDLK_s:
				transform->direction.setY(1);
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
				transform->direction.setY(0);
				sprite->Play("Idle Up");
				break;
			case SDLK_q:
				transform->direction.setX(0);
				sprite->Play("Idle Left");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_d:
				transform->direction.setX(0);
				sprite->Play("Idle Right");
				break;
			case SDLK_s:
				transform->direction.setY(0);
				sprite->Play("Idle Down");
				break;
			case SDLK_ESCAPE:
				GameManager::isRunning = false;
			default:
				break;
			}
		}
	}
};