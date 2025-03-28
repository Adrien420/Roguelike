#include <map>
#include <set>
#include "CardsManager.hpp"
#include "StatisticsComponent.hpp"
#include "Bonus.hpp"
#define PI 3.14

// Vecteurs de bonus
std::vector<Bonus> bonusBase, bonusProjectile;
// Bonus disponibles pour les joueurs
std::map<std::string,std::vector<std::vector<Bonus>>> bonusPlayer;
bool CardsManager::bonusInitialized = false;

void CardsManager::initBonus()
{
    // Suppression des anciens bonus initialisés et modifiés précédemment
    bonusBase.clear();
    bonusPlayer["player1"].clear();
    bonusPlayer["player2"].clear();
    
    // Bonus améliorant les statistiques
    Bonus bonusSpeed = Bonus("Up MVT speed (+20%)", -1, []() { upgradeStatPercent("speed", 0.2f)();} );
    bonusBase.emplace_back(bonusSpeed);
    Bonus bonusAttackSpeed = Bonus("Up ATK speed (+20%)", -1, []() { upgradeStatPercent("attackDuration", -0.2f)();} );
    bonusBase.emplace_back(bonusAttackSpeed);
    Bonus bonusHealth = Bonus("Up HP (+20%)", -1, []() { upgradeStatPercent("health", 0.2f)();} );
    bonusBase.emplace_back(bonusHealth);
    Bonus bonusDamages = Bonus("Up DMG sword(+20%)", -1, []() { upgradeStatPercent("damagesSword", 0.2f)();} );
    bonusBase.emplace_back(bonusDamages);
    Bonus bonusNbChoices= Bonus("Up NB choices (+1)", 2, []() { upgradeStat("nbCardsChoice", 1)();} );
    bonusBase.emplace_back(bonusNbChoices);

    // Bonus de projectiles
    Bonus bonusProjectiles = Bonus("Shoot projectiles", 1, []() { changeStat("hasProjectiles", true)();} );
    bonusBase.emplace_back(bonusProjectiles);
    Bonus bonusNbProjectiles= Bonus("Up NB projectiles (+2)", 3, []() { upgradeStat("nbProjectiles", 2)();} );
    bonusProjectile.emplace_back(bonusNbProjectiles);
    Bonus bonusDamagesProjectiles = Bonus("Up DMG projectiles (+20%)", -1, []() { upgradeStatPercent("damagesProjectiles", 0.2f)();} );
    bonusProjectile.emplace_back(bonusDamagesProjectiles);

    bonusPlayer["player1"].emplace_back(bonusBase);
    bonusPlayer["player2"].emplace_back(bonusBase);
}

void CardsManager::initBonusPlayer(std::string playerId)
{
    // Suppression des anciens bonus choisis (+ labels)
    selectedBonusIndexes[playerId].clear();
    txtTextures[playerId].clear();
    txtDestRects[playerId].clear();

    int bonusTypeIndex, bonusIndex;
    std::set<std::array<int, 2>> bonusesChosen;
    //Choix aléatoire de nbChoices bonus à proposer au player
    for(int i=0; i<nbChoices[playerId]; i++)
    {
        //Choix aléatoire des indexes d'un bonus parmis ceux disponibles pour le player
        bonusTypeIndex = rand() % bonusPlayer[playerId].size();
        bonusIndex = rand() % bonusPlayer[playerId][bonusTypeIndex].size();
        std::array<int, 2> bonusIndexes = {bonusTypeIndex, bonusIndex};

        //Vérification que le bonus n'a pas déjà été proposé
        std::set<std::array<int, 2>>::iterator it = bonusesChosen.find(bonusIndexes);
        int limitDuration = 100; //Evite une boucle infinie
        Uint32 startTimer = SDL_GetTicks();
        // On met une condition sur le temps pris pour trouver un bonus distinct => pour éviter de potentielles boucles infinies
        while(it != bonusesChosen.end() && SDL_GetTicks()-startTimer < limitDuration)
        {
            bonusTypeIndex = rand() % bonusPlayer[playerId].size();
            bonusIndex = rand() % bonusPlayer[playerId][bonusTypeIndex].size();
            bonusIndexes = {bonusTypeIndex, bonusIndex};
            it = bonusesChosen.find(bonusIndexes);
        }

        // Mémorisation du bonus choisi
        selectedBonusIndexes[playerId].emplace_back(bonusIndexes);
        bonusesChosen.insert(bonusIndexes);

        //Création de la texture pour le label
        Bonus bonus = bonusPlayer[playerId][bonusIndexes[0]][bonusIndexes[1]];
        std::string label = bonus.label;
        SDL_Rect txtDestRect;
        TTF_Font* font = GameManager::assets->GetFont("cardsFont");
        SDL_Color color = {255, 255, 255, 255};
        SDL_Texture* txtTexture = GameManager::assets->AddTxt(label, font, color, &txtDestRect, 1);
        txtTextures[playerId].emplace_back(txtTexture);
        txtDestRects[playerId].emplace_back(txtDestRect);
    }
    currentBonusIndexes[playerId] = selectedBonusIndexes[playerId][0];
}

void CardsManager::init()
{
    // Récupèration du nombre de choix pour chaque joueur
    nbChoices["player1"] = std::get<int>(GameManager::player1->getComponent<StatisticsComponent>().stats["nbCardsChoice"]);
    nbChoices["player2"] = std::get<int>(GameManager::player2->getComponent<StatisticsComponent>().stats["nbCardsChoice"]);
    
    if(!bonusInitialized)
    {
        initBonus();
        bonusInitialized = true;
    }
    // Choix d'une graine aléatoire
    srand(time(NULL));
    // Tirage aléatoire des bonus proposés
    initBonusPlayer("player1");
    initBonusPlayer("player2");

    indexSelection["player1"] = indexSelection["player2"] = 0;
    hasChosen["player1"] = hasChosen["player2"] = false;
    
    // UI pour indiquer quelle carte est actuellement sélectionnée
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
    // Un peu de trigo pour prendre en compte l'orientation des cartes
    int posY = offsetY[playerId] + destRect.w/2 * sin(abs(angleSelect * PI / 180));
    if(nbChoices[playerId] > 3)
        posY += destRect.w * sin(abs(30/(nbChoices[playerId]-1) * PI / 180));
    return posY;
}

void CardsManager::displayPlayerCards(std::string playerId, bool selected)
{
    for(int i=0; i<nbChoices[playerId]; i++)
	{
		// Angle calculé de sorte à avoir une présentation sous forme de "deck en main"
        angle = startAngle + i*30.0/(nbChoices[playerId]-1);
		destRect.x = startX[playerId] + i * (destRect.w + margin[playerId]);
		destRect.y = offsetY[playerId] + destRect.w/2 * sin(abs(angle * PI / 180));
		if((i == 0 || i == nbChoices[playerId]-1) && nbChoices[playerId] > 3)
			destRect.y += destRect.h * sin(abs(30/(nbChoices[playerId]-1) * PI / 180));

        if(selected) // Si la carte a été choisie
        {
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // Permet de modifier la couleur / opacité de la texture
            if(indexSelection[playerId] == i) // Carte sélectionnée
                SDL_SetTextureColorMod(texture, 255, 210, 0);
            else
                SDL_SetTextureAlphaMod(texture, 220);
        }
		SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);

        //Reset des modificateurs de couleur / opacité
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(texture, 255);

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

    SDL_Texture* menuCards = GameManager::assets->GetTexture("menuCards");
    SDL_Rect destRectMenu = {0, 0, 1280, 736};
	SDL_RenderCopyEx(GameManager::renderer, menuCards, NULL, &destRectMenu, 0, NULL, SDL_FLIP_NONE);

    if(GameManager::isVsIA && !hasChosen["player2"]) // Choix automatique et aléatoire pour l'IA
    {
        indexSelection["player2"] = rand()%nbChoices["player2"];
        currentBonusIndexes["player2"][0] = selectedBonusIndexes["player2"][indexSelection["player2"]][0];
        currentBonusIndexes["player2"][1] = selectedBonusIndexes["player2"][indexSelection["player2"]][1];
        select("player2");
    }

    if(hasChosen["player1"] && hasChosen["player2"]) // Si les 2 joueurs ont fait leur choix : nouvelle manche
    {
        initilized = false;
        GameManager::startNewRound();
    }

    displayPlayerCards("player1", hasChosen["player1"]);
    displayPlayerCards("player2", hasChosen["player2"]);

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

    if(!hasChosen["player1"])
        SDL_RenderCopyEx(GameManager::renderer, textureSelect, NULL, &destRectSelect, angleSelect, NULL, SDL_FLIP_NONE);
    if(!hasChosen["player2"])
        SDL_RenderCopyEx(GameManager::renderer, textureSelect, NULL, &destRectSelect2, angleSelect2, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(GameManager::renderer);
}

void CardsManager::changeCard(std::string playerId, SDL_Rect& destRectSelect_, double& angleSelect_, int side)
{
    if(hasChosen[playerId])
        return;
    
    //Changement de bonus sélectionné
    if((destRectSelect_.x == startX[playerId]) && (side == -1))
        indexSelection[playerId] = nbChoices[playerId]-1;
    else if((destRectSelect_.x == startX[playerId] + (nbChoices[playerId]-1) * (destRect.w + margin[playerId])) && (side == 1))
        indexSelection[playerId] = 0;
    else
        indexSelection[playerId] += side;
    
    currentBonusIndexes[playerId][0] = selectedBonusIndexes[playerId][indexSelection[playerId]][0];
    currentBonusIndexes[playerId][1] = selectedBonusIndexes[playerId][indexSelection[playerId]][1];
    
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
    if(hasChosen[playerId])
        return;
    // Récupèration par référence du bonus choisi à partir de ses indices dans les vecteurs de bonus
    std::vector<Bonus>& bonusVect = bonusPlayer[playerId][currentBonusIndexes[playerId][0]];
    Bonus& bonus = bonusVect[currentBonusIndexes[playerId][1]];
    std::string label = bonus.label;
    // On indique au bonus sur quel joueur son effet doit être appliqué
    if(playerId == "player1")
        bonus.player = GameManager::player1;
    else
        bonus.player = GameManager::player2;
    // Application du bonus
    bonus.applyBonus();
    if(bonus.nbUses == 0) // Suppression du bonus si épuisé
        bonusVect.erase(bonusVect.begin() + currentBonusIndexes[playerId][1]);
    if (bonusVect.empty()) // Si le vecteur de bonus (ex : bonusProjectiles) n'a plus de bonus disponibles, on le supprime
        bonusPlayer[playerId].erase(bonusPlayer[playerId].begin() + currentBonusIndexes[playerId][0]);
    hasChosen[playerId] = true;

    if(label == "Shoot projectiles")
        bonusPlayer[playerId].emplace_back(bonusProjectile);
}