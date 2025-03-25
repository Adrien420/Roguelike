#include <boost/test/unit_test.hpp>
#include "../src/Entity.hpp"
#include "../src/Bonus.hpp"

BOOST_AUTO_TEST_SUITE(BonusTests)

BOOST_AUTO_TEST_CASE(ApplyBonusTest) {
    // Création d'un joueur fictif pour tester les bonus
    Entity* player = new Entity(StatisticsComponent(100.0, 100.0, 100.0, 100.0, 5));
    Bonus::player = player;

    Bonus bonusDamages = Bonus("Up damages (+20%)", -1, []() { upgradeStatPercent("damagesSword", 0.2f)();} );
    bonusDamages.applyBonus();
    BOOST_CHECK_EQUAL(std::get<float>(player->getComponent<StatisticsComponent>().stats["damagesSword"]), 120.0);

    Bonus bonusNbChoices= Bonus("Up NB choices (+1)", 2, []() { upgradeStat("nbCardsChoice", 1)();} );
    bonusNbChoices.applyBonus();
    BOOST_CHECK_EQUAL(std::get<int>(player->getComponent<StatisticsComponent>().stats["nbCardsChoice"]), 6);

    Bonus bonusProjectiles = Bonus("Shoot projectiles", 1, []() { changeStat("hasProjectiles", true)();} );
    bonusProjectiles.applyBonus();
    BOOST_CHECK_EQUAL(std::get<bool>(player->getComponent<StatisticsComponent>().stats["hasProjectiles"]), true);
    BOOST_CHECK_EQUAL(bonusProjectiles.nbUses, 0);
}

BOOST_AUTO_TEST_SUITE_END()