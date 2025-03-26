#pragma once
#include <string>
#include "Components.hpp"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider{};  // Rectangle représentant la hitbox de l'entité
    std::string tag; // Tag utilisé pour identifier le type de collider (ex: "player", "enemy", "terrain")
    TransformComponent* transform = nullptr;
    int offsetX, offsetY; // Décalage du collider par rapport à l'entité
    int offsetW, offsetH; // Taille du collider

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

        if (tag == "None")
        {
            printf("%d %d %d %d\n", collider.x, collider.y, collider.w, collider.h);
        }
    }

    void draw() override
    {
        // Affiche le collider en rouge à l’écran si le mode DEBUG est activé
        if (GameManager::modeDEBUG)
        {
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 0, 0, 255);  // Mettre le collider en rouge
            SDL_RenderDrawRect(GameManager::renderer, &collider);
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 255, 255, 255);  // Réinitialiser la couleur du rendu au blanc 
        }
    }

    // Permet de vérifier si le collider entre en collision avec un autre ColliderComponent
    bool checkCollision(const ColliderComponent& other) const
    {
        const auto& recA = collider;
        const auto& recB = other.collider;

        return recA.x + recA.w >= recB.x &&
               recB.x + recB.w >= recA.x &&
               recA.y + recA.h >= recB.y &&
               recB.y + recB.h >= recA.y;
    }

    // Permet de vérifier si le collider entre en collision avec un rectangle SDL brut (map)
    bool checkCollision(const SDL_Rect& other) const
    {
        const auto& recA = collider;
        const auto& recB = other;

        return recA.x + recA.w >= recB.x &&
               recB.x + recB.w >= recA.x &&
               recA.y + recA.h >= recB.y &&
               recB.y + recB.h >= recA.y;
    }
};
