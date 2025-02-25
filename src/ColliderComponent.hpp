#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Components.hpp"
#include "Entity.hpp"
// #include "../TextureManager.h"

/*
Utiliser -DDEBUG dans les options du compilateur pour activer le mode debug
*/

class ColliderComponent : public Component
{
public:
    SDL_Rect collider{};
    std::string tag;
    TransformComponent* transform = nullptr;
    
#ifdef DEBUG
    SDL_Texture* tex = nullptr;
    SDL_Rect srcR{ 0, 0, 32, 32 };
    SDL_Rect destR{};
#endif

	// Constructeur EXPLICITE pour empêcher les conversions implicites
    explicit ColliderComponent(std::string t) 
        : tag(std::move(t)) {}	// std::move pour éviter des copies inutiles

    ColliderComponent(std::string t, int x, int y, int w, int h)
        : tag(std::move(t)), collider{ x, y, w, h } {}

    ~ColliderComponent() override
    {
#ifdef DEBUG
        if (tex) SDL_DestroyTexture(tex);
#endif
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }

        transform = &entity->getComponent<TransformComponent>();

#ifdef DEBUG
        tex = TextureManager::LoadTexture("assets/coltex.png");
#endif
    }

    void update() override
    {
        if (tag != "terrain")
        {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }

#ifdef DEBUG
        destR = { collider.x - Game::camera.x, collider.y - Game::camera.y, collider.w, collider.h };
#endif
    }

    void draw() override
    {
#ifdef DEBUG
        TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
#endif
    }
};
