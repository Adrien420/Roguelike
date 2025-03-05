#include "CardsManager.hpp"
#define PI 3.14

void CardsManager::init()
{
    texture = GameManager::assets->GetTexture("border");
    textureSelect = GameManager::assets->GetTexture("selection");
    SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);
    destRect.w = destRectSelect.w = imgWidth/2;
    destRect.h = destRectSelect.h = imgHeight/2;

    margin = 350 / nbChoices;
    totalWidth = nbChoices * destRect.w + (nbChoices - 1) * margin; // Largeur totale (cartes + espaces)
    startX = (1280 - totalWidth) / 2;
    startAngle = angleSelect = -15;

    destRectSelect.x = startX;
    destRectSelect.y = 30 + destRect.w/2 * sin(abs(angleSelect * PI / 180)) + destRect.w * sin(abs(30/(nbChoices-1) * PI / 180));
}

void CardsManager::choseCard()
{
    if(!initilized)
    {
        init();
        initilized = true;
    }
    SDL_RenderClear(GameManager::renderer);

    for(int i=0; i<nbChoices; i++)
	{
		angle = startAngle + i*30.0/(nbChoices-1);
		destRect.x = startX + i * (destRect.w + margin);
		destRect.y = 30 + destRect.w/2 * sin(abs(angle * PI / 180));
		if((i == 0 || i == nbChoices-1) && nbChoices > 3)
			destRect.y += destRect.w * sin(abs(30/(nbChoices-1) * PI / 180));
		SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
	}

    if (GameManager::event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = GameManager::event.key.keysym.sym;
        switch (key)
        {
            case SDLK_q:
                changeCard("player1",-1);
                break;
            case SDLK_LEFT:
                changeCard("player2",-1);
                break;
            case SDLK_d: 
                changeCard("player1",1);
                break;
            case SDLK_RIGHT:
                changeCard("player2",1);
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

    SDL_RenderPresent(GameManager::renderer);
}

void CardsManager::changeCard(std::string playerId, int side)
{
    //Orientation
    if((destRectSelect.x == startX) && (side == -1))
        angleSelect = 15;
    else if((destRectSelect.x == startX + (nbChoices-1) * (destRect.w + margin)) && (side == 1))
        angleSelect = -15;
    else if(side == 1)
        angleSelect += 30.0/(nbChoices-1);
    else
        angleSelect -= 30.0/(nbChoices-1);

     //Position horizontale
    if((destRectSelect.x == startX) && (side == -1))
        destRectSelect.x += (nbChoices-1) * (destRect.w + margin);
    else if((destRectSelect.x == startX + (nbChoices-1) * (destRect.w + margin)) && (side == 1))
        destRectSelect.x += -(nbChoices-1) * (destRect.w + margin);
    else
        destRectSelect.x += (destRect.w + margin) * side;

    //Position verticale
    destRectSelect.y = 30 + destRect.w/2 * sin(abs(angleSelect * PI / 180));
    if((destRectSelect.x == startX || destRectSelect.x == startX + (nbChoices-1) * (destRect.w + margin)) && nbChoices > 3)
        destRectSelect.y += destRect.w * sin(abs(30/(nbChoices-1) * PI / 180));
}

void CardsManager::select(std::string playerId)
{

}