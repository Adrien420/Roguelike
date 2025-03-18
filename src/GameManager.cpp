#include "GameManager.hpp"
#include "Components.hpp"

SDL_Renderer* GameManager::renderer = nullptr;
SDL_Event GameManager::event;
bool GameManager::isRunning = false;
bool GameManager::isPausing = false;
bool GameManager::chosingCards = false;
AssetManager* GameManager::assets = new AssetManager();
EntitiesManager GameManager::entitiesManager;
CardsManager GameManager::cardsManager;
Entity *GameManager::player1, *GameManager::player2;
Entity *UI;

GameManager::GameManager(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;

    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        isRunning = true;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Erreur SDL_ttf : " << TTF_GetError() << std::endl;
        isRunning = false;
    }

    SDL_Color white = { 255, 255, 255, 255 };

    assets->AddTexture("orc", "../assets/orc.png");
    assets->AddTexture("health", "../assets/health.png");
    assets->AddTexture("projectile", "../assets/projectile.png");
    assets->AddTexture("border", "../assets/card_border.jpeg");
    assets->AddTexture("selection", "../assets/selection.jpeg");
    assets->AddFont("mainFont","../assets/04B_30__.TTF", 24);

    // 🔹 Initialisation de la map après la création du renderer
    map = new Map("assets/map.txt", renderer);

    // Création des entités
    player1 = new Entity(StatisticsComponent(500, 100, 0.07, 100, 5), TransformComponent(0,0,64,64,2), SpriteComponent("orc", true), ColliderComponent("player1", 0, 0, 64, 64), KeyboardController("player1"), HealthComponent(100));
    player2 = new Entity(StatisticsComponent(500, 100, 0.07, 100, 3), TransformComponent(100,100,64,64,2), SpriteComponent("orc", true), ColliderComponent("player2", 17, 0, 30, 50), KeyboardController("player2"), HealthComponent(100));
    entitiesManager.addEntity(player1);
    entitiesManager.addEntity(player2);

    // Ajout d'un texte au jeu
    UI = new Entity(UILabelComponent("mainFont", "Hello", {255, 0, 0, 255}));
    UI->getComponent<UILabelComponent>().setPosition(500, 500);
    entitiesManager.addEntity(UI);
}

GameManager::~GameManager() {
    delete map;  // 🔹 Évite une fuite mémoire
}

void GameManager::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
            case SDLK_p:
                pause(!isPausing);
                break;
            case SDLK_o:
                chosingCards = !chosingCards;
                break;
            default:
                break;
        }
        break;
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void GameManager::update() {   
    entitiesManager.update();

    // 🔹 Vérifier les collisions entre le joueur et les obstacles de la map
    for (const auto& collider : map->getColliders()) {
        if (player1->getComponent<ColliderComponent>().checkCollision(collider)) {
            std::cout << "Collision avec un obstacle de la map !" << std::endl;

            // 🔹 Annule le déplacement si collision avec un obstacle
            player1->getComponent<TransformComponent>().position.x -= 
                player1->getComponent<TransformComponent>().direction.x * player1->getComponent<TransformComponent>().speed;
            player1->getComponent<TransformComponent>().position.y -= 
                player1->getComponent<TransformComponent>().direction.y * player1->getComponent<TransformComponent>().speed;
        }
    }

    // 🔹 Test de collision entre player1 et player2
    if (player1->getComponent<ColliderComponent>().checkCollision(player2->getComponent<ColliderComponent>())) {
        UI->getComponent<UILabelComponent>().setText("Collision");
    } else {
        UI->getComponent<UILabelComponent>().setText("Not Collision");
    }
}

void GameManager::render() {
    SDL_RenderClear(renderer);
    
    // 🔹 Affichage de la map
    map->DrawMap(renderer);
    
    entitiesManager.draw();
    SDL_RenderPresent(renderer);
}

void GameManager::reset()
{
    entitiesManager.reset();
}

void GameManager::clean()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}

void GameManager::pause(bool isPausing_)
{
    isPausing = isPausing_;
}
