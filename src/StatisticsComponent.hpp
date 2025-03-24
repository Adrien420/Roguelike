#ifndef STATISTICSCOMPONENT_HPP
#define STATISTICSCOMPONENT_HPP

#include <map>
#include <iostream>
#include <variant>
#include "SDL2/SDL.h"
#include "GameManager.hpp"

class StatisticsComponent : public Component
{
    public:
        using StatType = std::variant<int, float, bool>;
        std::map<std::string, StatType> stats = {
            {"attackDuration", StatType(500.0f)}, 
            {"damagesSword", StatType(100.0f)}, 
            {"speed", StatType(0.07f)}, 
            {"health", StatType(100.0f)}, 
            {"nbCardsChoice", StatType(3)}, 
            {"hasProjectiles", StatType(false)},
            {"nbProjectiles", StatType(1)},
            {"damagesProjectiles", StatType(20.0f)},
            {"isAttacking", StatType(false)}
        };

        StatisticsComponent(){}

        StatisticsComponent(float attackDuration_, float damages_, float speed_, float health_, int nbCardsChoice_)
        {
            stats["attackDuration"] = attackDuration_;
            stats["damages"] = damages_;
            stats["speed"] = speed_;
            stats["health"] = health_;
            stats["nbCardsChoice"] = nbCardsChoice_;
        }

};

#endif