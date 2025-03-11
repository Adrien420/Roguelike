#include <map>
#include "CardsManager.hpp"
#include "StatisticsComponent.hpp"
#include "Bonus.hpp"
#define PI 3.14

std::vector<Bonus> bonusBase;
std::map<std::string,std::vector<std::vector<Bonus>>> bonusPlayer;

void CardsManager::initBonus()
{
    // Bonus améliorant les statistiques
    Bonus bonusSpeed = Bonus("Up mvt speed(+20%)", -1, []() { upgradeStat("speed", 0.2f)();} );
    bonusBase.emplace_back(bonusSpeed);
    Bonus bonusAttackSpeed = Bonus("Up atk speed (+20%)", -1, []() { upgradeStat("attackDuration", -0.2f)();} );
    bonusBase.emplace_back(bonusAttackSpeed);

    // Bonus de projectiles
    Bonus bonusProjectiles = Bonus("Tirer des projectiles", 1, []() { changeStat("hasProjectiles", true)();} );
    bonusBase.emplace_back(bonusProjectiles);

    bonusPlayer["player1"].emplace_back(bonusBase);
    bonusPlayer["player2"].emplace_back(bonusBase);
}

void CardsManager::initBonusPlayer(std::string playerId)
{
    int bonusTypeIndex, bonusIndex;
    srand(time(NULL));
    //Choix aléatoire de nbChoices bonus à proposer au player
    for(int i=0; i<nbChoices[playerId]; i++)
    {
        //Choix aléatoire des indexes d'un bonus parmis ceux disponibles pour le player
        bonusTypeIndex = rand() % bonusPlayer[playerId].size();
        bonusIndex = rand() % bonusPlayer[playerId][bonusTypeIndex].size();
        std::array<int, 2> bonusIndexes = {bonusTypeIndex, bonusIndex};
        selectedBonusIndexes[playerId].emplace_back(bonusIndexes);

        //Création de la texture pour le label
        Bonus bonus = bonusPlayer[playerId][bonusIndexes[0]][bonusIndexes[1]];
        std::string label = bonus.label;
        SDL_Rect txtDestRect;
        SDL_Texture* txtTexture = GameManager::assets->AddTxt(label, &txtDestRect, 0.4);
        txtTextures[playerId].emplace_back(txtTexture);
        txtDestRects[playerId].emplace_back(txtDestRect);
    }
    currentBonusIndexes[playerId] = selectedBonusIndexes[playerId][0];
}

void CardsManager::init()
{
    nbChoices["player1"] = std::get<int>(GameManager::player1->getComponent<StatisticsComponent>().stats["nbCardsChoice"]);
    nbChoices["player2"] = std::get<int>(GameManager::player2->getComponent<StatisticsComponent>().stats["nbCardsChoice"]);
    
    if(!bonusInitialized)
    {
        initBonus();
        bonusInitialized = true;
    }
    initBonusPlayer("player1");
    initBonusPlayer("player2");
    
    texture = GameManager::assets->GetTexture("border");
    textureSelect = GameManager::assets->GetTexture("selection");
    SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);
    destRect.w = destRectSelect.w = destRectSelect2.w = imgWidth/1.9;
    destRect.h = destRectSelect.h = destRectSelect2.h = imgHeight/2.5;

    startX["player1"] = computeStartX("player1");
    startX["player2"] = computeStartX("player2");
    startAngle = angleSelect = angleSelect2 = -15;

    offsetY["player1"] = 30;
    offsetY["player2"] = 720/2;

    destRectSelect.x = startX["player1"];
    destRectSelect2.x = startX["player2"];
    destRectSelect.y = computeSelectY("player1");
    destRectSelect2.y = computeSelectY("player2");
}

int CardsManager::computeStartX(std::string playerId)
{
    margin[playerId] = 300 / nbChoices[playerId];
    int totalWidth = nbChoices[playerId] * destRect.w + (nbChoices[playerId] - 1) * margin[playerId]; // Largeur totale (cartes + espaces)
    return (1280 - totalWidth) / 2;
}

int CardsManager::computeSelectY(std::string playerId)
{
    int posY = offsetY[playerId] + destRect.w/2 * sin(abs(angleSelect * PI / 180));
    if(nbChoices[playerId] > 3)
        posY += destRect.w * sin(abs(30/(nbChoices[playerId]-1) * PI / 180));
    return posY;
}

void CardsManager::displayPlayerCards(std::string playerId)
{
    for(int i=0; i<nbChoices[playerId]; i++)
	{
		angle = startAngle + i*30.0/(nbChoices[playerId]-1);
		destRect.x = startX[playerId] + i * (destRect.w + margin[playerId]);
		destRect.y = offsetY[playerId] + destRect.w/2 * sin(abs(angle * PI / 180));
		if((i == 0 || i == nbChoices[playerId]-1) && nbChoices[playerId] > 3)
			destRect.y += destRect.h * sin(abs(30/(nbChoices[playerId]-1) * PI / 180));
		SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);

        //Label
        SDL_Rect txtRect = txtDestRects[playerId][i];
        txtRect.x = destRect.x + destRect.w/2 - txtRect.w/2;
        txtRect.y = destRect.y + destRect.h/2 - txtRect.h/2;
        SDL_RenderCopyEx(GameManager::renderer, txtTextures[playerId][i], NULL, &txtRect, angle, NULL, SDL_FLIP_NONE);
	}
}

void CardsManager::choseCard()
{
    if(!initilized)
    {
        init();
        initilized = true;
    }
    SDL_RenderClear(GameManager::renderer);

    displayPlayerCards("player1");
    displayPlayerCards("player2");

    if (GameManager::event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = GameManager::event.key.keysym.sym;
        switch (key)
        {
            case SDLK_q:
                changeCard("player1", destRectSelect, angleSelect,-1);
                break;
            case SDLK_LEFT:
                changeCard("player2", destRectSelect2, angleSelect2,-1);
                break;
            case SDLK_d: 
                changeCard("player1", destRectSelect, angleSelect,1);
                break;
            case SDLK_RIGHT:
                changeCard("player2", destRectSelect2, angleSelect2,1);
                break;
            case SDLK_SPACE:
                select("player1");
                break;
            case SDLK_m:
                select("player2");
                break;
            default:
                break;
        }
    }	

    SDL_RenderCopyEx(GameManager::renderer, textureSelect, NULL, &destRectSelect, angleSelect, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(GameManager::renderer, textureSelect, NULL, &destRectSelect2, angleSelect2, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(GameManager::renderer);
}

void CardsManager::changeCard(std::string playerId, SDL_Rect& destRectSelect_, double& angleSelect_, int side)
{
    //Orientation
    if((destRectSelect_.x == startX[playerId]) && (side == -1))
        angleSelect_ = 15;
    else if((destRectSelect_.x == startX[playerId] + (nbChoices[playerId]-1) * (destRect.w + margin[playerId])) && (side == 1))
        angleSelect_ = -15;
    else if(side == 1)
        angleSelect_ += 30.0/(nbChoices[playerId]-1);
    else
        angleSelect_ -= 30.0/(nbChoices[playerId]-1);

     //Position horizontale
    if((destRectSelect_.x == startX[playerId]) && (side == -1))
        destRectSelect_.x += (nbChoices[playerId]-1) * (destRect.w + margin[playerId]);
    else if((destRectSelect_.x == startX[playerId] + (nbChoices[playerId]-1) * (destRect.w + margin[playerId])) && (side == 1))
        destRectSelect_.x += -(nbChoices[playerId]-1) * (destRect.w + margin[playerId]);
    else
        destRectSelect_.x += (destRect.w + margin[playerId]) * side;

    //Position verticale
    destRectSelect_.y = offsetY[playerId] + destRect.w/2 * sin(abs(angleSelect_ * PI / 180));
    if((destRectSelect_.x == startX[playerId] || destRectSelect_.x == startX[playerId] + (nbChoices[playerId]-1) * (destRect.w + margin[playerId])) 
    && nbChoices[playerId] > 3)
        destRectSelect_.y += destRect.w * sin(abs(30/(nbChoices[playerId]-1) * PI / 180));
}

void CardsManager::select(std::string playerId)
{
    Bonus bonus = bonusPlayer[playerId][currentBonusIndexes[playerId][0]][currentBonusIndexes[playerId][1]];
    std::cout << bonus.label << std::endl;
    if(playerId == "player1")
        bonus.player = GameManager::player1;
    else
        bonus.player = GameManager::player2;
    bonus.applyBonus();
    std::cout << std::get<float>(GameManager::player1->getComponent<StatisticsComponent>().stats["attackDuration"]) << std::endl;
}