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
    int offsetX, offsetY; // Décalage du collider par rapport à l'entité
    int offsetW, offsetH; // Taille du collider
    
#ifdef DEBUG
    #define DEBUG_COLOR 255, 0, 0, 255 // Rouge
#endif

    ColliderComponent(std::string t, int x, int y, int w, int h)
        : tag(std::move(t)), offsetX(x), offsetY(y),  offsetW(w), offsetH(h) {}    // std::move pour éviter des copies inutiles

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
            collider.x = static_cast<int>(transform->position.x) + offsetX * transform->scale;
            collider.y = static_cast<int>(transform->position.y) + offsetY * transform->scale;
            collider.w = offsetW * transform->scale;
            collider.h = offsetH * transform->scale;
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
