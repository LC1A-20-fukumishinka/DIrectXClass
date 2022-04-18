#include "Game.h"
#include <memory>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	std::unique_ptr<Framework> game = std::make_unique<Game>() ;
	
	game->Run();

	return 0;
}
