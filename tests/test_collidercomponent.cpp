#include <boost/test/included/unit_test.hpp>
#include "ColliderComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"

// Fixture pour configurer une entité avec un collider
struct ColliderTestFixture {
    Entity entityA;
    Entity entityB;
    ColliderComponent& colliderA;
    ColliderComponent& colliderB;
    TransformComponent& transformA;
    TransformComponent& transformB;

    ColliderTestFixture()
        : transformA(entityA.addComponent<TransformComponent>(0, 0, 64, 64, 1.0)),
          transformB(entityB.addComponent<TransformComponent>(50, 50, 64, 64, 1.0)),
          colliderA(entityA.addComponent<ColliderComponent>("player", 0, 0, 64, 64)),
          colliderB(entityB.addComponent<ColliderComponent>("enemy", 0, 0, 64, 64)) {
        colliderA.init();
        colliderB.init();
    }
};

// Suite de tests
BOOST_FIXTURE_TEST_SUITE(ColliderComponentTests, ColliderTestFixture)

// Test de l'initialisation du collider
BOOST_AUTO_TEST_CASE(test_initialization) {
    BOOST_CHECK_EQUAL(colliderA.offsetX, 0);
    BOOST_CHECK_EQUAL(colliderA.offsetY, 0);
    BOOST_CHECK_EQUAL(colliderA.offsetW, 64);
    BOOST_CHECK_EQUAL(colliderA.offsetH, 64);
}

// Test de mise à jour des coordonnées du collider
BOOST_AUTO_TEST_CASE(test_update_collider_position) {
    transformA.position.x = 100;
    transformA.position.y = 200;
    colliderA.update();
    
    BOOST_CHECK_EQUAL(colliderA.collider.x, 100);
    BOOST_CHECK_EQUAL(colliderA.collider.y, 200);
    BOOST_CHECK_EQUAL(colliderA.collider.w, 64);
    BOOST_CHECK_EQUAL(colliderA.collider.h, 64);
}

// Test de collision entre deux colliders (collision attendue)
BOOST_AUTO_TEST_CASE(test_collision_detection) {
    colliderA.update();
    colliderB.update();
    
    BOOST_CHECK(colliderA.checkCollision(colliderB));
}

// Test de collision avec un `SDL_Rect` (collision attendue)
BOOST_AUTO_TEST_CASE(test_collision_with_sdl_rect) {
    SDL_Rect rect = {40, 40, 50, 50};  // Qui chevauche colliderA
    colliderA.update();

    BOOST_CHECK(colliderA.checkCollision(rect));
}

// Test d'absence de collision (entités trop éloignées)
BOOST_AUTO_TEST_CASE(test_no_collision) {
    transformB.position.x = 500;
    transformB.position.y = 500;
    colliderB.update();

    BOOST_CHECK(!colliderA.checkCollision(colliderB));
}

// Fin de la suite de tests
BOOST_AUTO_TEST_SUITE_END()
