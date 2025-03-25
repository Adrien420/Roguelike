#define BOOST_TEST_MODULE ProjectileComponentTest
#include <boost/test/included/unit_test.hpp>
#include "ProjectileComponent.hpp"
#include "TransformComponent.hpp"
#include "StatisticsComponent.hpp"
#include "Entity.hpp"
#include "SDL2/SDL.h"

// Mock des classes utilisées par ProjectileComponent
class MockTransformComponent : public TransformComponent {
public:
    Vector2D position;
    float speed;
    float scale;
    
    MockTransformComponent(Vector2D pos, float spd, float scl)
        : position(pos), speed(spd), scale(scl) {}
};

class MockStatisticsComponent : public StatisticsComponent {
    // Implémentation simplifiée pour le test
};

BOOST_AUTO_TEST_CASE(test_projectile_component_update) {
    // Créer une entité
    Entity entity;
    
    // Créer des composants et les ajouter à l'entité
    MockTransformComponent mockTransform(Vector2D{0, 0}, 10.0f, 1.0f);
    MockStatisticsComponent mockStats;
    
    entity.addComponent<MockTransformComponent>(Vector2D{0, 0}, 10.0f, 1.0f);
    entity.addComponent<MockStatisticsComponent>();
    
    // Créer un composant ProjectileComponent avec une direction donnée
    Vector2D direction{1.0f, 0.0f}; // Déplacement horizontal
    ProjectileComponent projectile(direction);
    
    // Assigner l'entité au projectile
    projectile.entity = &entity;
    
    // Initialisation du composant
    projectile.init();

    // Mise à jour du composant
    projectile.update();

    // Vérifier que la position a été mise à jour correctement (selon la vitesse et la direction)
    BOOST_CHECK_EQUAL(mockTransform.position.x, 10.0f); // 0 + (1 * 10.0f)
    BOOST_CHECK_EQUAL(mockTransform.position.y, 0); // Pas de changement en y
}


BOOST_AUTO_TEST_CASE(test_projectile_component_update) {
    // Créer une entité
    Entity entity;
    
    // Créer des composants et les ajouter à l'entité
    MockTransformComponent mockTransform(Vector2D{0, 0}, 10.0f, 1.0f);
    MockStatisticsComponent mockStats;
    
    entity.addComponent<MockTransformComponent>(Vector2D{0, 0}, 10.0f, 1.0f);
    entity.addComponent<MockStatisticsComponent>();
    
    // Créer un composant ProjectileComponent avec une direction donnée
    Vector2D direction{1.0f, 0.0f}; // Déplacement horizontal
    ProjectileComponent projectile(direction);
    
    // Assigner l'entité au projectile
    projectile.entity = &entity;
    
    // Initialisation du composant
    projectile.init();

    // Mise à jour du composant
    projectile.update();

    // Vérifier que la position a été mise à jour correctement
    BOOST_CHECK_EQUAL(mockTransform.position.x, 10.0f); // 0 + (1 * 10.0f)
    BOOST_CHECK_EQUAL(mockTransform.position.y, 0); // Pas de changement en y
}

BOOST_AUTO_TEST_CASE(test_projectile_component_draw) {
    // Créer une entité
    Entity entity;
    
    // Créer des composants et les ajouter à l'entité
    MockTransformComponent mockTransform(Vector2D{0, 0}, 10.0f, 1.0f);
    MockStatisticsComponent mockStats;
    
    entity.addComponent<MockTransformComponent>(Vector2D{0, 0}, 10.0f, 1.0f);
    entity.addComponent<MockStatisticsComponent>();
    
    // Créer un composant ProjectileComponent avec une direction donnée
    Vector2D direction{1.0f, 0.0f};
    ProjectileComponent projectile(direction);
    
    // Assigner l'entité au projectile
    projectile.entity = &entity;
    
    // Initialisation du composant
    projectile.init();

    // Utiliser un mock ou une technique similaire pour tester la fonction draw()
    // Ici, on vérifie simplement que la méthode ne plante pas
    BOOST_CHECK_NO_THROW(projectile.draw());
}
