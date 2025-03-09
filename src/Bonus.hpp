#ifndef BONUS_HPP
#define BONUS_HPP

#include <iostream>
#include <functional>
#include <variant>
#include "StatisticsComponent.hpp"

class Bonus
{
private:
    std::string label;
    int nbUses; // Nombre de fois que le bonus peut être choisi, = -1 si illimité
    std::function<void()> f;
public:
    template <typename Functor>
    Bonus(std::string label_, int nbUses_, Functor f_) : label(label_), nbUses(nbUses_), f(f_) {};

    void applyBonus()
    {
        f();
        if(nbUses > 0)
            nbUses--;
    }

    static Entity* player;

    ~Bonus() {}
};

//Foncteurs
class upgradeStat
{
    private:
        std::string stat;
        using StatType = std::variant<int, float, bool>;
        StatType value;
    public:
        upgradeStat(std::string stat_, StatType value_) : stat(stat_), value(value_) {}

        void operator()()
        {
            std::get<float>(Bonus::player->getComponent<StatisticsComponent>().stats[stat]) += std::get<float>(value);
        }

        ~upgradeStat() {}
};

class changeStat
{
    private:
        std::string stat;
        using StatType = std::variant<int, float, bool>;
        StatType value;
    public:
        changeStat(std::string stat_, StatType value_) : stat(stat_), value(value_) {}

        void operator()()
        {
            Bonus::player->getComponent<StatisticsComponent>().stats[stat] = value;
        }

        ~changeStat() {}
};


#endif