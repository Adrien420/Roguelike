#include <SDL2/SDL.h>
#include "GameManager.hpp"
#include "Entity.hpp"

int main(int argc, char *argv[])
{
    GameManager game_manager = GameManager("Roguelike", 1280, 736, false);

    const int FPS = 60;
	const float frameDelay = 1.0 / FPS;
    Uint32 frameStart;
	float frameTime;
    
    while(game_manager.isRunning)
    {   
        game_manager.handleEvents();

        if(game_manager.inHomeMenu)
        {
            game_manager.homeMenu();
        }

        if(game_manager.chosingCards)
        {
            game_manager.cardsManager.choseCard();
        }

        if(game_manager.inHomeMenu || game_manager.isPausing || game_manager.chosingCards)
            continue;

        frameStart = SDL_GetTicks();

		game_manager.update();
		game_manager.render();

        frameTime = (SDL_GetTicks() - frameStart) * 0.001;
		
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
    }
    game_manager.clean();

    return 0;
}