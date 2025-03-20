#ifndef HEALTHCOMPONENT_HPP
#define HEALTHCOMPONENT_HPP

#include "SDL2/SDL.h"
#include "Components.hpp"
#include "GameManager.hpp"

class HealthComponent : public Component
{
    private:
        TransformComponent *transform;
        StatisticsComponent *stats;
        SpriteComponent *sprite;
        SDL_Texture *texture = GameManager::assets->GetTexture("health");
        std::string playerId;
        SDL_Rect healthFill, destRect;
        int imgWidth, imgHeight;
        float fullHealth, healthPercent;
        Uint32 deathStart;
        bool isDead = false;
    
    public:
        float health;

        HealthComponent(std::string playerId_) : playerId(playerId_) {}

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            stats = &entity->getComponent<StatisticsComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            health = fullHealth = std::get<float>(stats->stats["health"]);

            // Récupération des dimensions de l'image utilisée pour la barre de vie
            SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);
        }

        void updateHealth(float amount)
        {
            health += amount;
            if(health < 0)
                health = 0;
        }

        void update() override
        {
            if(isDead && (SDL_GetTicks() - deathStart > sprite->deathAnimDuration))
            {
                if(playerId == "player1")
                    GameManager::endOfRound("player2");
                else
                    GameManager::endOfRound("player1");
            }
            else if(GameManager::inDeathAnimation)
                return;
            
            if(health > 0)
                updateHealth(0);
            else
            {
                isDead = true;
                deathStart = SDL_GetTicks();
                sprite->Play("Death");
                GameManager::inDeathAnimation = true;
            }

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

        void draw() override
        {
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 0, 0, 255); // Rouge
            SDL_RenderFillRect(GameManager::renderer, &healthFill);
            SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, 0, NULL, SDL_FLIP_NONE);
            SDL_SetRenderDrawColor(GameManager::renderer, 255, 255, 255, 255); // Réinitialser au blanc
        }

        void reset() override
        {
            health = fullHealth = std::get<float>(stats->stats["health"]);
            isDead = false;
        }
};

#endif