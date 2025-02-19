#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_JPP

#include "SDL2/SDL.h"
#include "GameManager.hpp"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent * transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::map<const char*, std::map<const char*, int>> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		animations["Idle"] = {{"index",0}, {"frames",3}, {"speed", 100}};
		animations["Walk"] = {{"index",1}, {"frames",8}, {"speed", 100}};

		Play("Idle");

		setTex(id);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string id)
	{
		texture = GameManager::assets->GetTexture(id);
	}

	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = transform->position.getX();
		destRect.y = transform->position.getY();
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		SDL_RenderCopyEx(GameManager::renderer, texture, &srcRect, &destRect, 0.0, NULL, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName]["frames"];
		animIndex = animations[animName]["index"];
		speed = animations[animName]["speed"];
	}

};

#endif