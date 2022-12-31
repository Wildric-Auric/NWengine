#ifndef _WINDLL	

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
		#ifdef NW_DLL_ENGINE
			//TODO::Find out how to create window in exe and process eveything in dll...
			int response = NW_RELOAD_DLL;
			while (response != NW_SHUTDOWN_DLL) {
				DllHandle engine = DllHandle("NWengine.dll");
				int(*DllRunFunc)() = (int(*)()) GetDllFunction(&engine, "DllRun");
				response = (*DllRunFunc)();
			}
		#else
			NWengine::Run();
			
		#endif
	#endif
	};
#endif
