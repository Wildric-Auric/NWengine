#include "TriangulationPrj.h"
#include "NWengine.h"
#include "Scene.h"
#include "SceneSet.h"
#include "Renderer.h"

namespace TriangulationPrj {

void Init() {
	Context::SetTitle("Triangulation Project");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	s.AddObject().AddComponent<SceneSet>();
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

void Render() { (*Renderer::defaultRenderer)(true); }

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}

}; // namespace TriangulationPrj
