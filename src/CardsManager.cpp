#include "CardsManager.hpp"
#define PI 3.14

void CardsManager::init()
{
    texture = GameManager::assets->GetTexture("border");
    SDL_QueryTexture(texture, NULL, NULL, &imgWidth, &imgHeight);
}

void CardsManager::endRound()
{
    init();
    SDL_Rect destRect;
    destRect.w = imgWidth/2;
    destRect.h = imgHeight/2;
    int margin = 350 / nbChoices;
    int totalWidth = nbChoices * destRect.w + (nbChoices - 1) * margin; // Largeur totale (cartes + espaces)
    int startX = (1280 - totalWidth) / 2;
    double startAngle = -15, angle;
    for(int i=0; i<nbChoices; i++)
	{
		angle = startAngle + i*30/(nbChoices-1);
		destRect.x = startX + i * (destRect.w + margin);
		destRect.y = 30 + destRect.w/2 * sin(abs(angle * PI / 180));
		if((i == 0 || i == nbChoices-1) && nbChoices > 3)
			destRect.y += destRect.w * sin(abs(30/(nbChoices-1) * PI / 180));
		SDL_RenderCopyEx(GameManager::renderer, texture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
	}
    SDL_RenderPresent(GameManager::renderer);
}