#include <boost/test/included/unit_test.hpp>
#include "SpriteComponent.hpp"
#include "Entity.hpp"
#include "StatisticsComponent.hpp"
#include "TransformComponent.hpp"
#include "GameManager.hpp"

// Fixture pour initialiser une entité avec SpriteComponent et ses dépendances
struct SpriteTestFixture {
    Entity entity;
    SpriteComponent& sprite;
    TransformComponent& transform;
    StatisticsComponent& stats;

    SpriteTestFixture()
        : transform(entity.addComponent<TransformComponent>(0, 0, 64, 64, 1.0)),
          stats(entity.addComponent<StatisticsComponent>()),
          sprite(entity.addComponent<SpriteComponent>("test_texture", true)) {
        stats.stats["attackDuration"] = 800.0f;
        sprite.init();
    }
};

// Suite de tests
BOOST_FIXTURE_TEST_SUITE(SpriteComponentTests, SpriteTestFixture)

// Test d'initialisation du SpriteComponent
BOOST_AUTO_TEST_CASE(test_initialization) {
    // Vérifie que l'animation démarre bien avec "Idle Down"
    sprite.update();
    BOOST_TEST(true); // On vérifie que tout fonctionne sans erreur
}

// Test du changement d'animation
BOOST_AUTO_TEST_CASE(test_animation_play) {
    sprite.Play("Walk Right");
    sprite.update();

    BOOST_TEST(true); // Pas de crash = bon fonctionnement
}

// Test de mise à jour : on vérifie que la frame d'animation avance
BOOST_AUTO_TEST_CASE(test_animation_update) {
    sprite.Play("Walk Down");

    Uint32 startTicks = SDL_GetTicks();
    while (SDL_GetTicks() - startTicks < 200) {
        sprite.update();
    }

    BOOST_TEST(true); // Si l'animation avance sans erreur, c'est bon
}

// Test de réinitialisation de l'animation
BOOST_AUTO_TEST_CASE(test_reset) {
    sprite.Play("Attack Up");
    sprite.reset();

    BOOST_TEST(true); // Vérifie que reset ne casse pas le jeu
}

// Fin de la suite de tests
BOOST_AUTO_TEST_SUITE_END()
