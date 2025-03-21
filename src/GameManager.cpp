#include "GameManager.hpp"
#include "Components.hpp"

SDL_Renderer* GameManager::renderer = nullptr;
SDL_Event GameManager::event;
bool GameManager::isRunning = false;
bool GameManager::inHomeMenu = true;
bool GameManager::isPausing = false;
bool GameManager::chosingCards = false;
bool GameManager::inDeathAnimation = false;
bool GameManager::isVsIA = false;
AssetManager* GameManager::assets = new AssetManager();
EntitiesManager GameManager::entitiesManager;
CardsManager GameManager::cardsManager;
Entity *GameManager::player1, *GameManager::player2;
Entity *UI;
int GameManager::nbwinRounds = 99;
std::map<std::string, int> GameManager::nbWinsPlayer = {{"player1", 0}, {"player2", 0}};

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

	map = Map("../assets/map.txt");

	// Home Menu
	destRectButtonPlayer = {1280/2-buttonPlayerWidth/2, 736/3-buttonPlayerHeight/2, buttonPlayerWidth, buttonPlayerHeight};
	destRectButtonIA = {1280/2-buttonPlayerWidth/2, 2*736/3-buttonPlayerHeight/2, buttonPlayerWidth, buttonPlayerHeight};

	assets->AddTexture("menu", "../assets/menu.png");
	assets->AddTexture("button", "../assets/button.png");
	assets->AddTexture("orc", "../assets/orc.png");
	assets->AddTexture("health", "../assets/health.png");
	assets->AddTexture("projectile", "../assets/projectile.png");
	assets->AddTexture("border", "../assets/card_border.jpeg");
	assets->AddTexture("selection", "../assets/selection.jpeg");
	assets->AddTexture("menuCards", "../assets/cardsSelectBackground.png");

	assets->AddFont("mainFont","../assets/04B_30__.TTF", 24);
	assets->AddFont("cardsFont","../assets/SF.ttf", 20);

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
			{
				isVsIA = false;
				initGame();
			}
			if(mouseX >= destRectButtonIA.x && mouseX <= destRectButtonIA.x + destRectButtonIA.w
				&& mouseY >= destRectButtonIA.y && mouseY <= destRectButtonIA.y + destRectButtonIA.h)
			{
				isVsIA = true;
				initGame();
			}
				
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
	entitiesManager.refresh();
	entitiesManager.update();

	// Vérifier les collisions entre les joueurs et les obstacles de la map
	for (const auto& collider : map.getColliders()) {
		if (player1->getComponent<ColliderComponent>().checkCollision(collider)) {
			std::cout << "Collision avec un obstacle de la map !" << std::endl;

			// Annule le déplacement si le joueur entre en collision avec un obstacle
			player1->getComponent<TransformComponent>().position.x -= 
				player1->getComponent<TransformComponent>().direction.x * player1->getComponent<TransformComponent>().speed;
			player1->getComponent<TransformComponent>().position.y -= 
				player1->getComponent<TransformComponent>().direction.y * player1->getComponent<TransformComponent>().speed;
		}
	}

	bool isCollision = false;

	// Récupération des composants collider des joueurs
	auto& player1Collider = player1->getComponent<ColliderComponent>();
	auto& player2Collider = player2->getComponent<ColliderComponent>();

	// Vérifier la collision entre les deux joueurs
	if (player1Collider.checkCollision(player2Collider)) {
		isCollision = true;
	}

	// Parcourir toutes les entités pour vérifier les collisions avec "sword" et "projectile"
	for (Entity* entity : entitiesManager.entities) {
		if (entity->hasComponent<ColliderComponent>()) {
			auto& entityCollider = entity->getComponent<ColliderComponent>();

			// Vérifier les collisions avec player1
			if (entityCollider.tag == "sword" || entityCollider.tag == "projectile"){
				if (entity->playerId != "player1" && entityCollider.checkCollision(player1Collider)) {
					isCollision = true;
					float damage = std::get<float>(player2->getComponent<StatisticsComponent>().stats["damages"]);
					player1->getComponent<HealthComponent>().updateHealth(-damage);
					entity->destroy();
				}
				if (entity->playerId != "player2" && entityCollider.checkCollision(player2Collider)) {
					isCollision = true;
					float damage = std::get<float>(player1->getComponent<StatisticsComponent>().stats["damages"]);
					player2->getComponent<HealthComponent>().updateHealth(-damage);
					entity->destroy();
				}
			}
		}
	}

	// Mise à jour de l'UI
	std::string newText = isCollision ? "Collision" : "Not Collision";
	if (UI->getComponent<UILabelComponent>().getText() != newText) {
		UI->getComponent<UILabelComponent>().setText(newText);
	}


	// Récupérer la taille de la fenêtre
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	// Supprimer les entités non désirables
    for (Entity* e : entitiesManager.entities) 
    {
		// Supprime les projectiles hors écran
        if (e->label == "projectile")
        {
            auto& transform = e->getComponent<TransformComponent>();

            // Vérifier si le projectile est hors des limites de l'écran
            if (transform.position.x < 0 || transform.position.x > windowWidth ||
                transform.position.y < 0 || transform.position.y > windowHeight) 
            {
                e->destroy();
            }
        }
		// Supprime les épées hors attaque
		if (e->label == "sword")
        {
            bool isAttackingPlayer1 = std::get<bool>(player1->getComponent<StatisticsComponent>().stats["isAttacking"]);
			bool isAttackingPlayer2 = std::get<bool>(player2->getComponent<StatisticsComponent>().stats["isAttacking"]);
			std::string swordID = e->playerId;
			if(!isAttackingPlayer1 && swordID == "player1")
			{
				e->destroy();
			}
			if(!isAttackingPlayer2 && swordID == "player2")
			{
				e->destroy();
			}
        }
    }
	entitiesManager.refresh();
}

void GameManager::render() {
    SDL_RenderClear(renderer);
    
    // Affichage de la map
    map.DrawMap(renderer);
    
    entitiesManager.draw();
    SDL_RenderPresent(renderer);
}

void GameManager::reset()
{
	GameManager::inDeathAnimation = false;
	// Supprime les entités "projectile" et "sword"
	for (Entity* e : entitiesManager.entities) {
		if (e->label == "projectile" || e->label == "sword") {
			e->destroy();
		}
	}
	entitiesManager.refresh();
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

void GameManager::createPlayers()
{
	// Supprime les entités "player" déjà créées
	for (Entity* e : entitiesManager.entities) {
		if (e->label == "player") {
			e->destroy();
		}
	}
	entitiesManager.refresh();
	
	float baseSpeed = 0.1;
	// Attention, l'ordre d'ajout des composants a une importance, car certains dépendent des autres, et chaque composant est ajouté et initialisé dans l'ordre de passage en paramètre
	player1 = new Entity(StatisticsComponent(500, 100, baseSpeed, 500, 3), TransformComponent(300,336,64,64,2), SpriteComponent("orc", true), ColliderComponent("player1", 17, 0, 30, 50), KeyboardController("player1"), HealthComponent("player1"));
	if(!isVsIA)
		player2 = new Entity(StatisticsComponent(500, 100, baseSpeed, 500, 3), TransformComponent(916,336,64,64,2), SpriteComponent("orc", true), ColliderComponent("player2", 17, 0, 30, 50), KeyboardController("player2"), HealthComponent("player2"));
	else
		player2 = new Entity(StatisticsComponent(500, 100, baseSpeed, 500, 3), TransformComponent(916,336,64,64,2), SpriteComponent("orc", true), ColliderComponent("player2", 17, 0, 30, 50), IAControllerComponent("player2", player1), HealthComponent("player2"));
	player1->label = "player";
	player2->label = "player";
	player1->playerId = "player1";
	player2->playerId = "player2";
	entitiesManager.addEntity(player1);
	entitiesManager.addEntity(player2);
}

void GameManager::homeMenu()
{
	SDL_RenderClear(renderer);

	SDL_Texture* menu = GameManager::assets->GetTexture("menu");
    SDL_Rect destRectMenu = {0, 0, 1280, 736};
	SDL_RenderCopyEx(GameManager::renderer, menu, NULL, &destRectMenu, 0, NULL, SDL_FLIP_NONE);

	SDL_Texture* buttonPlayer = GameManager::assets->GetTexture("button");
	SDL_RenderCopyEx(GameManager::renderer, buttonPlayer, NULL, &destRectButtonPlayer, 0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(GameManager::renderer, buttonPlayer, NULL, &destRectButtonIA, 0, NULL, SDL_FLIP_NONE);

	SDL_Rect txtDestRectPlayer, txtDestRectIA;
	TTF_Font* font = GameManager::assets->GetFont("mainFont");
	SDL_Color color = {0, 0, 0, 255};
	SDL_Texture* txtTexturePLayer = GameManager::assets->AddTxt("Player vs Player", font, color, &txtDestRectPlayer, 1);
	txtDestRectPlayer.x = 1280/2 - txtDestRectPlayer.w/2;
	txtDestRectPlayer.y = 736/3 - txtDestRectPlayer.h/2;
	SDL_RenderCopyEx(GameManager::renderer, txtTexturePLayer, NULL, &txtDestRectPlayer, 0, NULL, SDL_FLIP_NONE);
	SDL_Texture* txtTextureIA = GameManager::assets->AddTxt("Player vs IA", font, color, &txtDestRectIA, 1);
	txtDestRectIA.x = 1280/2 - txtDestRectIA.w/2;
	txtDestRectIA.y = 2*736/3 - txtDestRectIA.h/2;
	SDL_RenderCopyEx(GameManager::renderer, txtTextureIA, NULL, &txtDestRectIA, 0, NULL, SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);
}

void GameManager::initGame()
{
	createPlayers();
	inHomeMenu = false;
}

void GameManager::pause(bool isPausing_)
{
    isPausing = isPausing_;
}

void GameManager::endOfRound(std::string playerId)
{
	GameManager::nbWinsPlayer[playerId]++;
	std::cout << playerId << " a remporté la manche : " 
	<< GameManager::nbWinsPlayer[playerId] << " / " << GameManager::nbwinRounds << " !" << std::endl;
	if(GameManager::nbWinsPlayer[playerId] >= GameManager::nbwinRounds)
	{
		std::cout << playerId << " a remporté la partie !" << std::endl;
		// Réinitialisation complète du jeu
		cardsManager.bonusInitialized = false;
		GameManager::nbWinsPlayer["player1"] = 0;
		GameManager::nbWinsPlayer["player2"] = 0;
		GameManager::reset();
		inHomeMenu = true;
		return;
	}
	chosingCards = true;
}

void GameManager::startNewRound()
{
	GameManager::reset();
	chosingCards = false;
}