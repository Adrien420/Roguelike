#include <SDL2/SDL.h>
#include "GameManager.hpp"
#include "Entity.hpp"

int main(int argc, char *argv[])
{
    GameManager game_manager = GameManager("Roguelike", 600, 400, false);

    const int FPS = 60;
	const int frameDelay = 1 / FPS;
    Uint32 frameStart;
	int frameTime;
    
    while(game_manager.isRunning)
    {
        frameStart = SDL_GetTicks();
        
        game_manager.handleEvents();
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