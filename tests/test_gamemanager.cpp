#include <boost/test/unit_test.hpp>
#include "../src/GameManager.hpp"

// Mock SDL2
struct SDLFixture {
    SDLFixture() { SDL_Init(SDL_INIT_VIDEO); TTF_Init(); }
    ~SDLFixture() { TTF_Quit(); SDL_Quit(); }
};

BOOST_FIXTURE_TEST_SUITE(GameManagerTests, SDLFixture)

// Test de reset des entités (sword et projectile supprimés)
BOOST_AUTO_TEST_CASE(ResetTest) {
    GameManager game("Test Window", 800, 600, false);
    auto sword = new Entity();
    sword->label = "sword";
    GameManager::entitiesManager.addEntity(sword);

    auto projectile = new Entity();
    projectile->label = "projectile";
    GameManager::entitiesManager.addEntity(projectile);

    BOOST_CHECK_EQUAL(GameManager::entitiesManager.entities.size(), 3); //3 et non 2 car entité UI créée dans le constructeur
    GameManager::reset();
    BOOST_CHECK_EQUAL(GameManager::entitiesManager.entities.size(), 1);
}

// Test de la création des joueurs
BOOST_AUTO_TEST_CASE(CreatePlayersTest) {
    GameManager game("Test Window", 800, 600, false);
    GameManager::isVsIA = false;
    game.createPlayers();
    BOOST_CHECK(GameManager::player1 != nullptr);
    BOOST_CHECK(GameManager::player2 != nullptr);
    BOOST_CHECK_EQUAL(GameManager::player1->playerId, "player1");
    BOOST_CHECK_EQUAL(GameManager::player2->playerId, "player2");
}

// Test de endOfRound : gestion des points de victoire
BOOST_AUTO_TEST_CASE(EndOfRoundTest) {
    GameManager game("Test Window", 800, 600, false);
    GameManager::nbwinRounds = 2;
    GameManager::nbWinsPlayer["player1"] = 0;

    // Fin de manche
    game.endOfRound("player1");
    BOOST_CHECK_EQUAL(GameManager::nbWinsPlayer["player1"], 1);
    BOOST_CHECK(GameManager::chosingCards);

    // Fin de partie
    game.endOfRound("player1");
    BOOST_CHECK_EQUAL(GameManager::nbWinsPlayer["player1"], 0);
    BOOST_CHECK(GameManager::inHomeMenu);
}

// Test de startNewRound
BOOST_AUTO_TEST_CASE(StartNewRoundTest) {
    GameManager game("Test Window", 800, 600, false);
    GameManager::chosingCards = true;
    GameManager::startNewRound();
    BOOST_CHECK_EQUAL(GameManager::chosingCards, false);
}

BOOST_AUTO_TEST_SUITE_END()