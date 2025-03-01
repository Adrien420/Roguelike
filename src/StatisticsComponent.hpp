#ifndef STATISTICSCOMPONENT_HPP
#define STATISTICSCOMPONENT_HPP

#include "SDL2/SDL.h"
#include "Components.hpp"
#include "GameManager.hpp"

class StatisticsComponent : public Component
{
    public:
        float attackDuration;
        float damages;
        float speed;
        float health;
        bool hasProjectiles = true;

        StatisticsComponent(float attackDuration_, float damages_, float speed_, float health_)
        {
            attackDuration = attackDuration_;
            damages = damages_;
            health = health_;
            speed = speed_;
        }

};

#endif