#include "GameManager.hpp"
#include "Entity.hpp"
#include "Components.hpp"

SDL_Renderer* GameManager::renderer = nullptr;
SDL_Event GameManager::event;
bool GameManager::isRunning = false;
AssetManager* GameManager::assets = new AssetManager();
EntitiesManager entitiesManager;
Entity player, player2;

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

	// Attention, l'ordre d'ajout des composants a une importance, car certains dépendent des autres, et chaque composant est ajouté et initialisé dans l'ordre de passage en paramètre
	player = Entity(TransformComponent(0,0,64,64,2), StatisticsComponent(800, 100, 0.07, 100), SpriteComponent("orc", true), KeyboardController("player1"), HealthComponent(100));
	player2 = Entity(TransformComponent(100,100,64,64,2), StatisticsComponent(800, 100, 0.07, 100), SpriteComponent("orc", true), KeyboardController("player2"), HealthComponent(100));
	entitiesManager.addEntity(std::move(player));
	entitiesManager.addEntity(std::move(player2));

}

GameManager::~GameManager()
{}


void GameManager::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void GameManager::update()
{	
	entitiesManager.refresh();
	entitiesManager.update();
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
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}