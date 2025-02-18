#pragma once

// #include "../Game.h"
// #include "ECS.h"
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
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_z:
				transform->velocity.y = -1;
				sprite->Play("Walk Up");
				break;
			case SDLK_q:
				transform->velocity.x = -1;
				sprite->Play("Walk Left");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->Play("Walk Right");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->Play("Walk Down");
				break;
			default:
				break;
			}
		}
	
		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_z:
				transform->velocity.y = 0;
				sprite->Play("Idle Up");
				break;
			case SDLK_q:
				transform->velocity.x = 0;
				sprite->Play("Idle Left");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->Play("Idle Right");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->Play("Idle Down");
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
		}
	}
};