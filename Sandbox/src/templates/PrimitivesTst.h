#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"
#include "TriangleRenderer.h"
#include "CircleRenderer.h"


namespace PrimitivesTst {

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
    GameObject& cam = s.AddObject();
    GameObject& tri = s.AddObject();
    GameObject& cir = s.AddObject();

    Camera* camC = cam.AddComponent<Camera>();
    camC->Use();
    camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
    camC->ChangeOrtho(720,480);

    TriangleRenderer* triR = tri.AddComponent<TriangleRenderer>();
    //triR->SetBaseRight({200.0,-100.0f});
    CircleRenderer* cr = cir.AddComponent<CircleRenderer>();
    cr->SetPosition({-250,0});
    cr->SetRadius(32);
    cr->SetRenderingAA(0.25);

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
};

