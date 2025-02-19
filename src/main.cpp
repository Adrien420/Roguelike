#include <SDL2/SDL.h>
#include "GameManager.hpp"
#include "Entity.hpp"

int main(int argc, char *argv[])
{
    GameManager game_manager = GameManager("Roguelike", 600, 400, false);
    
    while(game_manager.isRunning)
    {
        game_manager.handleEvents();
		game_manager.update();
		game_manager.render();
    }
    game_manager.clean();

    return 0;
}