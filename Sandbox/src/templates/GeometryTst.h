#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"
#include "LineRenderer.h"

namespace GeometryTst {

struct Data {
    LineRenderer* lines[8];
};

v4f exts[8] = {
    {0.0,0.0, 56.0,-56.0},
    {-53.0, -50, 100,50}
};

Data data;

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
    //-------------- 
    for (int i = 0; i < 2; i++) {
        data.lines[i] = s.AddObject().AddComponent<LineRenderer>();
        data.lines[i]->SetWidth(5);
        data.lines[i]->SetExt({exts[i].x, exts[i].y}, {exts[i].z, exts[i].w});
    }


    //-------------
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() { 
        

    (*Renderer::defaultRenderer)(true); 
}

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}
}; // namespace Simple
