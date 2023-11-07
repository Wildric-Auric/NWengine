#pragma once

/*


					 /$$   /$$ /$$      /$$                               /$$
					| $$$ | $$| $$  /$ | $$                              |__/
					| $$$$| $$| $$ /$$$| $$  /$$$$$$  /$$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$
					| $$ $$ $$| $$/$$ $$ $$ /$$__  $$| $$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
					| $$  $$$$| $$$$_  $$$$| $$$$$$$$| $$  \ $$| $$  \ $$| $$| $$  \ $$| $$$$$$$$
					| $$\  $$$| $$$/ \  $$$| $$_____/| $$  | $$| $$  | $$| $$| $$  | $$| $$_____/
					| $$ \  $$| $$/   \  $$|  $$$$$$$| $$  | $$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
					|__/  \__/|__/     \__/ \_______/|__/  |__/ \____  $$|__/|__/  |__/ \_______/
										    /$$  \ $$
									           |  $$$$$$/
										    \______/

*/


#include "Primitives.h"
#include "Inputs.h"
#include "Context.h"
#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
#include "Batch.h"
#include "Text.h"
#include "NWproj.h"

#include "Utilities.h"
#include "Globals.h"
#include "NWTime.h"




#define NW_NO_MANGLING extern "C"
#ifdef NW_DLL
#define NW_EXPORT     __declspec(dllexport)
#else
#define NW_EXPORT
#endif
#define NW_PREFIX NW_NO_MANGLING NW_EXPORT

enum class ON_MAIN_CALL_LOCATION {
	
	InitBegin,			//Executed just after context initialization
	InitEnd,			//Executed at the end of engine init, after initialization of engine system
	FrameBegin,			//Executed at beginning of each frame
	FrameIntermediate,  //Executed each frame just before updating context buffer
	FrameEnd,			//Executed each frame after updating context 
	Destroy				//Called before destruction of engine systems
};

extern std::unordered_map<ON_MAIN_CALL_LOCATION, std::vector<void(*)()>> functionMap;
extern GameObject NWengineDefaultRendererContainer;

NW_PREFIX int  NWengineInit();
NW_PREFIX void NWengineLoop();
NW_PREFIX void NWengineShutdown();
NW_PREFIX bool NWenginePushFunction(ON_MAIN_CALL_LOCATION loc, void(*func)());
NW_PREFIX bool NWenginePopFunction(ON_MAIN_CALL_LOCATION loc, void(*func)() = nullptr);
