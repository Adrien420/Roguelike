#include <SDL2/SDL.h>
#include "src/GameManager.hpp"
#include "src/Entity.hpp"

int main(int argc, char *argv[])
{
    GameManager game_manager = GameManager("Roguelike", 600, 400, false);
    
    while(game_manager.getIsRunning())
    {
        game_manager.handleEvents();
		game_manager.update();
		game_manager.render();
    }
    game_manager.clean();

    return 0;
}