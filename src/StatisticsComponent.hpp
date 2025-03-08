#ifndef STATISTICSCOMPONENT_HPP
#define STATISTICSCOMPONENT_HPP

#include "SDL2/SDL.h"
#include "GameManager.hpp"

class StatisticsComponent : public Component
{
    public:
        float attackDuration = 500;
        float damages = 100;
        float speed = 0.07;
        float health = 100;
        bool hasProjectiles = true;
        int nbCardsChoice = 3;

        StatisticsComponent(){}

        StatisticsComponent(float attackDuration_, float damages_, float speed_, float health_, int nbCardsChoice_)
        {
            attackDuration = attackDuration_;
            damages = damages_;
            health = health_;
            speed = speed_;
            nbCardsChoice = nbCardsChoice_;
        }

};

#endif