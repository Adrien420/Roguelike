#include "GameManager.hpp"
#include "Components.hpp"

SDL_Renderer* GameManager::renderer = nullptr;
SDL_Event GameManager::event;
bool GameManager::isRunning = false;
bool GameManager::inHomeMenu = true;
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
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);	// Blanc
		}

		isRunning = true;
	}

	if (TTF_Init() == -1) {
        std::cerr << "Erreur SDL_ttf : " << TTF_GetError() << std::endl;
        isRunning = false;
    }


	if (!map.init(window))
	{
		std::cerr << "Erreur lors de l'initialisation de la map." << std::endl;
		isRunning = false;
	}

	// Home Menu
	destRectButtonPlayer = {1280/2-buttonPlayerWidth/2, 720/2-buttonPlayerHeight/2, buttonPlayerWidth, buttonPlayerHeight};

	assets->AddTexture("menu", "../assets/menu.png");
	assets->AddTexture("button", "../assets/button.png");
	assets->AddTexture("orc", "../assets/orc.png");
	assets->AddTexture("health", "../assets/health.png");
	assets->AddTexture("projectile", "../assets/projectile.png");
	assets->AddTexture("border", "../assets/card_border.jpeg");
	assets->AddTexture("selection", "../assets/selection.jpeg");

	assets->AddFont("mainFont","../assets/04B_30__.TTF", 24);
	assets->AddFont("cardsFont","../assets/SF.ttf", 20);

	// Attention, l'ordre d'ajout des composants a une importance, car certains dépendent des autres, et chaque composant est ajouté et initialisé dans l'ordre de passage en paramètre
	player1 = new Entity(StatisticsComponent(500, 100, 0.07, 100, 5), TransformComponent(0,0,64,64,2), SpriteComponent("orc", true), ColliderComponent("player1", 17, 0, 30, 50), KeyboardController("player1"), HealthComponent());
	player2 = new Entity(StatisticsComponent(500, 100, 0.07, 100, 3), TransformComponent(100,100,64,64,2), SpriteComponent("orc", true), ColliderComponent("player2", 17, 0, 30, 50), KeyboardController("player2"), HealthComponent());
	entitiesManager.addEntity(player1);
	entitiesManager.addEntity(player2);

	// Ajout d'un texte au jeu
	UI = new Entity(UILabelComponent("mainFont", "Hello", {255, 0, 0, 255}));
	UI->getComponent<UILabelComponent>().setPosition(500, 500);
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
			default:
				break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
		if(inHomeMenu)
		{
			if(mouseX >= destRectButtonPlayer.x && mouseX <= destRectButtonPlayer.x + destRectButtonPlayer.w
				&& mouseY >= destRectButtonPlayer.y && mouseY <= destRectButtonPlayer.y + destRectButtonPlayer.h)
				initGame();
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
	if (player1->getComponent<ColliderComponent>().checkCollision(player2->getComponent<ColliderComponent>()))
	{
		UI->getComponent<UILabelComponent>().setText("Collision");
	}
	else
	{
		UI->getComponent<UILabelComponent>().setText("Not Collision");
	}
}

void GameManager::render()
{
	SDL_RenderClear(renderer);
	map.render(renderer);
	entitiesManager.draw();
	SDL_RenderPresent(renderer);
}

void GameManager::reset()
{
	entitiesManager.entities.erase(
		std::remove_if(entitiesManager.entities.begin(), entitiesManager.entities.end(),
			[](Entity* e) {
				if (e->label == "projectile") {
					delete e; // Libération mémoire
					return true; // Supprime de la liste
				}
				return false;
			}),
		entitiesManager.entities.end()
	);
	entitiesManager.reset();
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

	TTF_Quit();
    SDL_Quit();
}

void GameManager::homeMenu()
{
	SDL_RenderClear(renderer);

	SDL_Texture* menu = GameManager::assets->GetTexture("menu");
    SDL_Rect destRectMenu = {0, 0, 1280, 720};
	SDL_RenderCopyEx(GameManager::renderer, menu, NULL, &destRectMenu, 0, NULL, SDL_FLIP_NONE);

	SDL_Texture* buttonPlayer = GameManager::assets->GetTexture("button");
	SDL_RenderCopyEx(GameManager::renderer, buttonPlayer, NULL, &destRectButtonPlayer, 0, NULL, SDL_FLIP_NONE);
	SDL_Rect txtDestRectPvp;
	TTF_Font* font = GameManager::assets->GetFont("mainFont");
	SDL_Color color = {0, 0, 0, 255};
	SDL_Texture* txtTexture = GameManager::assets->AddTxt("PvP", font, color, &txtDestRectPvp, 1);
	txtDestRectPvp.x = 1280/2 - txtDestRectPvp.w/2;
	txtDestRectPvp.y = 720/2 - txtDestRectPvp.h/2;
	SDL_RenderCopyEx(GameManager::renderer, txtTexture, NULL, &txtDestRectPvp, 0, NULL, SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);
}

void GameManager::initGame()
{
	inHomeMenu = false;
}

void GameManager::pause(bool isPausing_)
{
	isPausing = isPausing_;
}

void GameManager::endOfRound()
{
	chosingCards = true;
}

void GameManager::startNewRound()
{
	GameManager::reset();
	chosingCards = false;
}