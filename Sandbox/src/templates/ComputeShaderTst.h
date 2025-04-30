#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"
#include "Shader.h"

namespace ComputeShaderTst {

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);
	s.Start();

	Loader<ComputeShader> shader;
	shader.LoadFromFileOrGetFromCache((void*)"C:\\nwengine\\NWengine\\UtilScripts\\Test.comp.shader",
									  "C:\\nwengine\\NWengine\\UtilScripts\\Test.comp.shader", 0);

	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() { (*Renderer::defaultRenderer)(true); }

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}
}; // namespace ComputeShaderTst
