
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
			DllHandle* engine = nullptr;
			while (response != NW_SHUTDOWN_DLL) {
				std::vector<std::string> temp = GetNWlist("CurrentProj.NWlist");
				if (temp.size() < 1) Globals::dllDir = "";	
				else				 Globals::dllDir = temp[0];
				delete engine;
				FileMove(Globals::dllDir + "NWengine.dll", Globals::dllDir + "NWengine_temp.dll");
				engine = new DllHandle((Globals::dllDir + "NWengine.dll").c_str());
				if (engine->Get() == nullptr) {
					delete engine;
					engine = new DllHandle("NWengine.dll");
				}
				int(*DllRunFunc)() = (int(*)()) GetDllFunction(engine, "DllRun");
				response = (*DllRunFunc)();
			}
		#else
			NWengine::Run();
		#endif
	#endif
	};
#endif
