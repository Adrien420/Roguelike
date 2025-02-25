#pragma once
#include <string>
#include "Components.hpp"
#include "TextureManager.hpp"

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
    #define DEBUG_COLOR 255, 0, 0, 255 // Rouge
#endif

	// Constructeur EXPLICITE pour empêcher les conversions implicites
    explicit ColliderComponent(std::string t) 
        : tag(std::move(t)), collider{0, 0, 0, 0} {}	// std::move pour éviter des copies inutiles

    ColliderComponent(std::string t, int x, int y, int w, int h)
        : tag(std::move(t)), collider{x, y, w, h} {}

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }

        transform = &entity->getComponent<TransformComponent>();
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
    }

    void draw() override
    {
#ifdef DEBUG
        SDL_SetRenderDrawColor(GameManager::renderer, DEBUG_COLOR);
        SDL_RenderDrawRect(GameManager::renderer, &collider);
#endif
    }

    bool checkCollision(const ColliderComponent& other) const
    {
        const auto& recA = collider;
        const auto& recB = other.collider;

        return recA.x + recA.w >= recB.x &&
               recB.x + recB.w >= recA.x &&
               recA.y + recA.h >= recB.y &&
               recB.y + recB.h >= recA.y;
    }
};
