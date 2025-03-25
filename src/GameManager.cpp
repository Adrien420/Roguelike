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
SoundManager* GameManager::soundManager = &SoundManager::getInstance();
Entity *GameManager::player1, *GameManager::player2;
Entity *UI;
int GameManager::nbwinRounds = 4;
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

	map = Map("../assets/map.txt", "../assets/map_overlay.txt");

	// Home Menu
	destRectButtonPlayer = {1280/2-buttonPlayerWidth/2, 736/3-buttonPlayerHeight/2, buttonPlayerWidth, buttonPlayerHeight};
	destRectButtonIA = {1280/2-buttonPlayerWidth/2, 2*736/3-buttonPlayerHeight/2, buttonPlayerWidth, buttonPlayerHeight};

	assets->AddTexture("menu", "../assets/menu.png");
	assets->AddTexture("button", "../assets/button.png");
	assets->AddTexture("p1Keys", "../assets/player1Keys.png");
	assets->AddTexture("orc", "../assets/orc.png");
	assets->AddTexture("health", "../assets/health.png");
	assets->AddTexture("projectile", "../assets/projectile.png");
	assets->AddTexture("border", "../assets/card_border.jpeg");
	assets->AddTexture("selection", "../assets/selection.jpeg");
	assets->AddTexture("menuCards", "../assets/cardsSelectBackground.png");

	assets->AddFont("mainFont","../assets/04B_30__.TTF", 24);
	assets->AddFont("cardsFont","../assets/SF.ttf", 20);

	GameManager::soundManager->loadMusic("mainBGM", "../assets/Sounds/mainBGM.mp3");
	GameManager::soundManager->loadMusic("battleBGM", "../assets/Sounds/battleBGM.mp3");
	GameManager::soundManager->loadMusic("cardsBGM", "../assets/Sounds/cardsBGM.mp3");
	GameManager::soundManager->loadSoundEffect("swordSE", "../assets/Sounds/swordSE.mp3");
	GameManager::soundManager->loadSoundEffect("projectileSE", "../assets/Sounds/projectileSE.mp3");
	GameManager::soundManager->loadSoundEffect("damageSE", "../assets/Sounds/damageSE.mp3", 128*0.5);
	GameManager::soundManager->playMusic("mainBGM");
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

void GameManager::preventMvt(TransformComponent& playerPos, SDL_Rect playerCollider, SDL_Rect obstacleCollider)
{
	int distX = playerCollider.x + playerCollider.w - obstacleCollider.x;
	int distX2 = obstacleCollider.x + obstacleCollider.w - playerCollider.x;
	int distY = playerCollider.y + playerCollider.h - obstacleCollider.y;
	int distY2 = obstacleCollider.y + obstacleCollider.h - playerCollider.y;
	int margin = 5;

	if(playerPos.direction.x != 0)
	{
		if(distX >= 0 && distX <= obstacleCollider.w/2 && abs(distY) > margin && abs(distY2) > margin)
			playerPos.position.x -= distX;
		else if(distX2 >= 0 && distX2 <= obstacleCollider.w/2 && abs(distY) > margin && abs(distY2) > margin)
			playerPos.position.x += distX2;
	}
	else if(playerPos.direction.y != 0)
	{
		if(distY >= 0 && distY <= obstacleCollider.h/2 && abs(distX) > margin && abs(distX2) > margin)
			playerPos.position.y -= distY;
		else if(distY2 >= 0 && distY2 <= obstacleCollider.h/2 && abs(distX) > margin && abs(distX2) > margin)
			playerPos.position.y += distY2;
	}
}

void GameManager::update()
{	
	entitiesManager.refresh();
	entitiesManager.update();

	// Récupération des composants collider et transform des joueurs
	auto& player1Collider = player1->getComponent<ColliderComponent>();
	auto& player2Collider = player2->getComponent<ColliderComponent>();
	auto& player1Transform = player1->getComponent<TransformComponent>();
	auto& player2Transform = player2->getComponent<TransformComponent>();

	// Vérifier les collisions entre les joueurs et les obstacles de la map
	for (const auto& collider : map.getColliders()) {
		if (player1Collider.checkCollision(collider)) {
			// Empêche le déplacement dans cette direction
			preventMvt(player1Transform, player1Collider.collider, collider);
		}
		if (player2Collider.checkCollision(collider)) {
			// Empêche le déplacement dans cette direction
			preventMvt(player2Transform, player2Collider.collider, collider);
		}
	}

	// Parcourir toutes les entités pour vérifier les collisions avec "sword" et "projectile"
	for (Entity* entity : entitiesManager.entities) {
		if (entity->hasComponent<ColliderComponent>()) {
			auto& entityCollider = entity->getComponent<ColliderComponent>();

			// Vérifier les collisions avec player
			if (entityCollider.tag == "sword" || entityCollider.tag == "projectile") {
				std::string damageType = (entityCollider.tag == "sword") ? "damagesSword" : "damagesProjectiles";

				if (entity->playerId != "player1" && entityCollider.checkCollision(player1Collider)) {
					float damage = std::get<float>(player2->getComponent<StatisticsComponent>().stats[damageType]);
					player1->getComponent<HealthComponent>().updateHealth(-damage);
					entity->destroy();
					// Éviter le chevauchement des damageSE
					if (!std::get<bool>(player1->getComponent<StatisticsComponent>().stats["isTakingDamages"])){
						GameManager::soundManager->playSoundEffect("damageSE");
						player1->getComponent<StatisticsComponent>().stats["isTakingDamages"] = true;
					}
					
				}
				if (entity->playerId != "player2" && entityCollider.checkCollision(player2Collider)) {
					float damage = std::get<float>(player1->getComponent<StatisticsComponent>().stats[damageType]);
					player2->getComponent<HealthComponent>().updateHealth(-damage);
					entity->destroy();
					// Éviter le chevauchement des damageSE
					if (!std::get<bool>(player2->getComponent<StatisticsComponent>().stats["isTakingDamages"])){
						GameManager::soundManager->playSoundEffect("damageSE");
						player2->getComponent<StatisticsComponent>().stats["isTakingDamages"] = true;
					}
				}
			}
		}
	}
	if (std::get<bool>(player1->getComponent<StatisticsComponent>().stats["isTakingDamages"])){
		player1->getComponent<StatisticsComponent>().stats["isTakingDamages"] = false;
	}
	if (std::get<bool>(player2->getComponent<StatisticsComponent>().stats["isTakingDamages"])){
		player2->getComponent<StatisticsComponent>().stats["isTakingDamages"] = false;
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

    soundManager->clean(); 

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
	
	float baseSpeed = 0.3;
	// Attention, l'ordre d'ajout des composants a une importance, car certains dépendent des autres, et chaque composant est ajouté et initialisé dans l'ordre de passage en paramètre
	player1 = new Entity(StatisticsComponent(500, 100, baseSpeed, 500, 3), TransformComponent(300,336,64,64,2), SpriteComponent("orc", true), ColliderComponent("player1", 20, 8, 24, 34), KeyboardController("player1"), HealthComponent("player1"));
	if(!isVsIA)
		player2 = new Entity(StatisticsComponent(500, 100, baseSpeed, 500, 3), TransformComponent(916,336,64,64,2), SpriteComponent("orc", true), ColliderComponent("player2", 20, 8, 24, 34), KeyboardController("player2"), HealthComponent("player2"));
	else
		player2 = new Entity(StatisticsComponent(500, 100, baseSpeed, 500, 3), TransformComponent(916,336,64,64,2), SpriteComponent("orc", true), ColliderComponent("player2", 20, 8, 24, 34), IAControllerComponent("player2", player1), HealthComponent("player2"));
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

	SDL_Texture* player1Keys = GameManager::assets->GetTexture("p1Keys");
    SDL_Rect destRectP1Keys = {0, 486, 250, 250};
	SDL_RenderCopyEx(GameManager::renderer, player1Keys, NULL, &destRectP1Keys, 0, NULL, SDL_FLIP_NONE);

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
	GameManager::soundManager->stopMusic();
	GameManager::soundManager->playMusic("battleBGM");
}

void GameManager::pause(bool isPausing_)
{
    isPausing = isPausing_;
}

void GameManager::endOfRound(std::string playerId)
{
	GameManager::soundManager->stopAllSounds();
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
		GameManager::soundManager->playMusic("mainBGM");
		inHomeMenu = true;
		return;
	}
	GameManager::soundManager->playMusic("cardsBGM");
	chosingCards = true;
}

void GameManager::startNewRound()
{
	GameManager::reset();
	GameManager::soundManager->stopMusic();
	GameManager::soundManager->playMusic("battleBGM");
	chosingCards = false;
}