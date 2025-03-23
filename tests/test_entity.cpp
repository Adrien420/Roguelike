#define BOOST_TEST_MODULE EntityTests
#include <boost/test/included/unit_test.hpp>
#include "../src/Entity.hpp"

// Composant factice pour les tests
class DummyComponent : public Component {
public:
    bool initialized = false;
    void init() override { initialized = true; }
    void update() override {}
    void draw() override {}
};

BOOST_AUTO_TEST_CASE(AddComponentTest)
{
    Entity entity;
    auto& comp = entity.addComponent<DummyComponent>();
    
    BOOST_CHECK(entity.hasComponent<DummyComponent>());
    BOOST_CHECK(comp.initialized == true);
}

BOOST_AUTO_TEST_CASE(RemoveInactiveEntitiesTest)
{
    EntitiesManager manager;
    Entity* e1 = new Entity();
    Entity* e2 = new Entity();
    
    manager.addEntity(e1);
    manager.addEntity(e2);

    BOOST_CHECK(manager.entities.size() == 2);
    
    e1->destroy();
    manager.refresh();

    BOOST_CHECK(manager.entities.size() == 1);
    BOOST_CHECK(manager.entities[0] == e2);
}

BOOST_AUTO_TEST_CASE(UpdateAndResetTest)
{
    Entity entity;
    auto& comp = entity.addComponent<DummyComponent>();

    entity.update();
    entity.reset();
    // Pas de changement particulier à vérifier ici, juste s'assurer que ça ne plante pas
    BOOST_CHECK(true);
}