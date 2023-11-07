#include "NWengine.h"
#include "Gui.h"

int main() {
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd,           Gui::Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameBegin,        Gui::Begin);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Gui::Update);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Gui::Render);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::Destroy,		   Gui::Destroy);

	NWengineInit();
	NWengineLoop();
	NWengineShutdown();

	return 0;
}