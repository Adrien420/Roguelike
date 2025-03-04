#include "GameManager.hpp"
#include "Components.hpp"

SDL_Renderer* GameManager::renderer = nullptr;
SDL_Event GameManager::event;
bool GameManager::isRunning = false;
bool GameManager::isPausing = false;
AssetManager* GameManager::assets = new AssetManager();
EntitiesManager GameManager::entitiesManager;
CardsManager GameManager::cardsManager;
Entity *player, *player2, *UI;

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

	if (!map.init(window))
	{
		std::cerr << "Erreur lors de l'initialisation de la map." << std::endl;
		isRunning = false;
	}

	SDL_Color white = { 255, 255, 255, 255 };

	assets->AddTexture("orc", "../assets/orc.png");
	assets->AddTexture("health", "../assets/health.png");
	assets->AddTexture("projectile", "../assets/projectile.png");
	assets->AddTexture("border", "../assets/card_border.jpeg");

	// Attention, l'ordre d'ajout des composants a une importance, car certains dépendent des autres, et chaque composant est ajouté et initialisé dans l'ordre de passage en paramètre
	player = new Entity(TransformComponent(0,0,64,64,2), StatisticsComponent(500, 100, 0.07, 100), SpriteComponent("orc", true), ColliderComponent("player1", 0, 0, 64, 64), KeyboardController("player1"), HealthComponent(100));
	player2 = new Entity(TransformComponent(100,100,64,64,2), StatisticsComponent(500, 100, 0.07, 100), SpriteComponent("orc", true), ColliderComponent("player2", 100, 100, 64, 64), KeyboardController("player2"), HealthComponent(100));
	entitiesManager.addEntity(player);
	entitiesManager.addEntity(player2);

	// Ajout d'un texte au jeu
	UI = new Entity(UILabelComponent("../assets/font.ttf", 24, "Nothing", {255, 0, 0, 255}));
	UI->getComponent<UILabelComponent>().setPosition(500, 50);
	entitiesManager.addEntity(UI);
}

GameManager::~GameManager()
{}


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
				if(!isPausing)
					pause(true);
				cardsManager.endRound();
				break;
			default:
				break;
		}
		break;
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void GameManager::update()
{	
	//entitiesManager.refresh();
	entitiesManager.update();

	// Test de collision entre player1 et player2
	if (player->getComponent<ColliderComponent>().checkCollision(player2->getComponent<ColliderComponent>()))
	{
		UI->getComponent<UILabelComponent>().setText("Collision !");
	}
}

void GameManager::render()
{
	SDL_RenderClear(renderer);
	map.render(renderer);
	entitiesManager.draw();
	SDL_RenderPresent(renderer);
}

void GameManager::clean()
{
	if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;  // Évite l'accès à des pointeurs sauvages
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;  // Évite l'accès à des pointeurs sauvages
    }

    SDL_Quit();
}

void GameManager::pause(bool isPausing_)
{
	isPausing = isPausing_;
}