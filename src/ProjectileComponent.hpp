#ifndef PROJECTILECOMPONENT_HPP
#define PROJECTILECOMPONENT_HPP

#include "SDL2/SDL.h"
#include "Components.hpp"
#include "GameManager.hpp"

// Classe qui gére le comportement d’un projectile (déplacement, affichage)
class ProjectileComponent : public Component
{
    private:
        TransformComponent *transform;
        StatisticsComponent *stats;
        Vector2D direction;
        SDL_Texture *texture = GameManager::assets->GetTexture("projectile");
        SDL_Rect destRect;
        int imgWidth, imgHeight;
        float speed;
    
    public:
        ProjectileComponent(Vector2D direction_)
        {
            direction = direction_;
        }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            stats = &entity->getComponent<StatisticsComponent>();
            speed = transform->speed;

            // Récupération des dimensions de l'image utilisée pour le projectile
            SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);
        }

        void update() override
        {
            transform->position.x += direction.x * speed;
            transform->position.y += direction.y * speed;

            destRect.x = transform->position.x;
            destRect.y = transform->position.y;
            destRect.w = imgWidth * transform->scale;
            destRect.h = imgHeight * transform->scale;
        }

        void draw() override
        {
            SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, 0, NULL, SDL_FLIP_NONE);
        }
};

#endif