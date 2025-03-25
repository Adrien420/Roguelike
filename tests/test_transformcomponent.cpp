#include <boost/test/included/unit_test.hpp>
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "StatisticsComponent.hpp"

// Fixture pour initialiser une entité avec TransformComponent et StatisticsComponent
struct TransformTestFixture {
    Entity entity;
    TransformComponent& transform;

    // Initialisation de la fixture
    TransformTestFixture()
        : transform(entity.addComponent<TransformComponent>(0, 0, 64, 64, 1.0)) {
        // Ajout du composant StatisticsComponent à l'entité
        StatisticsComponent& stats = entity.addComponent<StatisticsComponent>();
        stats.stats["speed"] = 0.1f;

        // Initialisation des composants
        transform.init();
    }
};

// Suite de tests pour TransformComponent
BOOST_FIXTURE_TEST_SUITE(TransformComponentTests, TransformTestFixture)

// Test d'initialisation
BOOST_AUTO_TEST_CASE(test_initialization) {
    BOOST_TEST(transform.position.x == 0);
    BOOST_TEST(transform.position.y == 0);
    BOOST_TEST(transform.speed == 0.1f); // Vérifie que la vitesse est bien récupérée des stats
}

// Test de mise à jour du mouvement
BOOST_AUTO_TEST_CASE(test_movement) {
    transform.direction = Vector2D(1, 0); // Déplacement vers la droite
    transform.update(); // Appliquer l'update()

    // La position x doit augmenter de `speed`
    BOOST_TEST(transform.position.x == 0.1f, boost::test_tools::tolerance(0.001));
    BOOST_TEST(transform.position.y == 0); // Pas de mouvement en Y
}

// Test de réinitialisation de la position
BOOST_AUTO_TEST_CASE(test_reset) {
    entity.playerId = "player1"; // Simuler un joueur 1
    transform.reset(); // Appliquer reset()

    BOOST_TEST(transform.position.x == 300); // Vérifier position X après reset
    BOOST_TEST(transform.position.y == 368); // Vérifier position Y après reset
}

// Fin de la suite de tests
BOOST_AUTO_TEST_SUITE_END()
