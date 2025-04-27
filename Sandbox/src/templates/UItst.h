#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"
#include "UIWindow.h"
#include "UISys.h"


namespace UITst {

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
    GameObject& cam   = s.AddObject();
    GameObject& uwin  = s.AddObject();
    GameObject& uwin2 = s.AddObject();
    GameObject& uwin3 = s.AddObject();
    Camera* camC = cam.AddComponent<Camera>();
    camC->Use();
    camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
    camC->ChangeOrtho(500,500);
    camC->GetFbo()->GenDepthStencilBuffer();
    Renderer::defaultRenderer->SetStretch({1.0,1.0});

    uwin.AddComponent<UIWindow>()->SetTitle("Hello Window");
    uwin2.AddComponent<UIWindow>();
    uwin2.GetComponent<Transform>()->Translate({-150,0});
    uwin3.AddComponent<UIWindow>();
    uwin3.GetComponent<Transform>()->Translate({150,0});

    s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
    (*Renderer::defaultRenderer)(true);
}

void Run() {
        Context::_glInfo.maxVersion = 4;
        Context::_glInfo.minVersion = 6;
        Context::WINDOW_WIDTH = 800;
        Context::WINDOW_HEIGHT= 800;

        NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
        NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
        NWengineInit();
        NWengineLoop();
        NWengineShutdown();
    }
};
