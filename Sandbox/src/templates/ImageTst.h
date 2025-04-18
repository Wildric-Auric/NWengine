#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"


namespace ImageTst{

void MakeImageAndSave() {
    Image im;
    im.height = 500;
    im.width = 500;
    im.channels = 3;
    im.alpha = 1;
    im.Alloc();
    for (int i = 0; i < 500; ++i) {
        for (int j = 0; j < 500; ++j) {
            uint8 dist = Clamp(10000.0 / (fVec2(i,j) - fVec2(250,250)).magnitude(),0.0,255.0);
            im.Write({i,j}, {dist,dist,dist,255});
        }
    }
    im.SaveToFile("a.png");
    im.Clean();
}

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
    GameObject& cam = s.AddObject();
    Camera* camC = cam.AddComponent<Camera>();
    camC->Use();
    camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
    camC->ChangeOrtho(720,480);
    s.Start();
    
    MakeImageAndSave();
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
