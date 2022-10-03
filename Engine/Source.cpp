#ifdef NW_GAME_BUILD
	#include "Game.h"
#else
	#include "NWengine.h"
#endif
int main()
{	
#ifdef NW_GAME_BUILD
	Game::Run();
#else
	NWengine::Run();
#endif
};
