#include <boost/test/included/unit_test.hpp>
#include "../src/HealthComponent.hpp"
#include "../src/Entity.hpp"
#include "../src/StatisticsComponent.hpp"
#include "../src/TransformComponent.hpp"
#include "../src/SpriteComponent.hpp"
#include "../src/GameManager.hpp"

// Fixture pour initialiser une entité avec HealthComponent et ses dépendances
struct HealthTestFixture {
    Entity entity;
    HealthComponent& health;
    TransformComponent& transform;
    StatisticsComponent& stats;
    SpriteComponent& sprite;

    HealthTestFixture()
        : transform(entity.addComponent<TransformComponent>(0, 0, 64, 64, 1.0)),
          stats(entity.addComponent<StatisticsComponent>()),
          sprite(entity.addComponent<SpriteComponent>("test_texture", true)),
          health(entity.addComponent<HealthComponent>("player1")) {
        stats.stats["health"] = 100.0f;
        health.init();
    }
};

// Suite de tests
BOOST_FIXTURE_TEST_SUITE(HealthComponentTests, HealthTestFixture)

// Test d'initialisation de la santé
BOOST_AUTO_TEST_CASE(test_initialization) {
    BOOST_CHECK_EQUAL(health.health, 100.0f);  // Santé initiale correcte
}

// Test de la mise à jour de la santé (dégâts)
BOOST_AUTO_TEST_CASE(test_health_decrease) {
    health.updateHealth(-30.0f);
    BOOST_CHECK_EQUAL(health.health, 70.0f);
}

// Test de la mise à jour de la santé (soin)
BOOST_AUTO_TEST_CASE(test_health_increase) {
    health.updateHealth(-30.0f);
    health.updateHealth(20.0f);
    BOOST_CHECK_EQUAL(health.health, 90.0f);
}

// Test que la santé ne devient jamais négative
BOOST_AUTO_TEST_CASE(test_health_cannot_go_negative) {
    health.updateHealth(-200.0f);
    BOOST_CHECK_EQUAL(health.health, 0.0f);
}

// Test que la mort déclenche bien l'animation de mort
BOOST_AUTO_TEST_CASE(test_death_triggers_animation) {
    health.updateHealth(-100.0f);
    health.update();
    BOOST_TEST(GameManager::inDeathAnimation);
}

// Test du reset : la santé doit revenir à son niveau initial
BOOST_AUTO_TEST_CASE(test_reset) {
    health.updateHealth(-50.0f);
    health.reset();
    BOOST_CHECK_EQUAL(health.health, 100.0f);
}

// Fin de la suite de tests
BOOST_AUTO_TEST_SUITE_END()
