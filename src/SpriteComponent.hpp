#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_JPP

#include "SDL2/SDL.h"
#include "GameManager.hpp"
#include "StatisticsComponent.hpp"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent *transform;
	StatisticsComponent *stats;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int frameTime = 100; // Durée d'une frame de l'animation en ms
	Uint32 animStartTime = 0;
	std::string currentAnimation = "";

public:

	int animIndex = 0;
	std::map<const std::string, std::map<const char*, int>> animations;

	SpriteComponent() = default;

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;
		setTex(id);
	}

	~SpriteComponent() {}

	void setTex(std::string id)
	{
		texture = GameManager::assets->GetTexture(id);
	}

	void setAnimation(std::string type, int index, int nbFrames, float animDuration)
	{
		animations[type + " Down"] = {{"index",index}, {"frames",nbFrames}, {"frameTime", animDuration/nbFrames}};
		animations[type + " Up"] = {{"index",index+1}, {"frames",nbFrames}, {"frameTime", animDuration/nbFrames}};
		animations[type + " Left"] = {{"index",index+2}, {"frames",nbFrames}, {"frameTime", animDuration/nbFrames}};
		animations[type + " Right"] = {{"index",index+3}, {"frames",nbFrames}, {"frameTime", animDuration/nbFrames}};
	}

	void initAnimations()
	{
		setAnimation("Idle", 0, 4, 400);
		setAnimation("Walk", 4, 8, 800);
		setAnimation("Attack", 8, 8, stats->attackDuration);

		Play("Idle Down");
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		stats = &entity->getComponent<StatisticsComponent>();

		if(animated)
		{
			initAnimations();
		}

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			// Grâce à animStartTime, on s'assure que la nouvelle animation seléctionnée démarre bien à la première frame
			int elapsedTime = SDL_GetTicks() - animStartTime;
			// Dès que frameTime ms passsent, elapsedTime / frameTime augmente de 1, et la frame suivante de l'animation est sélectionnée
			// La durée totale de l'animation est donc de frameTime * frames ms
			srcRect.x = srcRect.w * static_cast<int>((elapsedTime / frameTime) % frames);
			//std::cout << elapsedTime << " / " << srcRect.x / srcRect.w << std::endl;
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = transform->position.x;
		destRect.y = transform->position.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		SDL_RenderCopyEx(GameManager::renderer, texture, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);
	}

	void Play(const std::string animName)
	{
		frames = animations[animName]["frames"];
		animIndex = animations[animName]["index"];
		frameTime = animations[animName]["frameTime"];
		if(animName != currentAnimation)
			animStartTime = SDL_GetTicks();
		currentAnimation = animName;
	}

};

#endif