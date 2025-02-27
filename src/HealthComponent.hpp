#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "SDL2/SDL.h"
#include "Components.hpp"
#include "GameManager.hpp"

class HealthComponent : public Component
{
    private:
        TransformComponent * transform;
        SDL_Texture *texture = GameManager::assets->GetTexture("health");
        SDL_Rect healthFill, destRect;
        int imgWidth, imgHeight;
        float fullHealth, healthPercent;
    
    public:
        float health;

        HealthComponent(float health_)
        {
            health = fullHealth = health_;
        }

        void init()
        {
            transform = &entity->getComponent<TransformComponent>();

            // Récupération des dimensions de l'image utilisée pour la barre de vie
            SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);
        }

        void updateHealth(float amount)
        {
            health += amount;
        }

        void update()
        {
            if(health > 0)
                updateHealth(-0.005);
            else
                std::cout << "Dead" << std::endl;

            healthPercent = health / fullHealth;

            destRect.w = imgWidth/20 * transform->scale;
            healthFill.w = (destRect.w - 7 * transform->scale) * healthPercent;
            destRect.h = imgHeight/20 * transform->scale;
            healthFill.h = destRect.h - 4.5 * transform->scale;

            destRect.x = healthFill.x = transform->position.x + 10;
            healthFill.x += 5.5 * transform->scale;
            destRect.y = healthFill.y = transform->position.y - 20;
            healthFill.y += 2.5 * transform->scale;
        }

        void draw()
        {
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(GameManager::renderer, &healthFill);
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 255, 0, 255);
            SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, 0, NULL, SDL_FLIP_NONE);
        }
};

#endif