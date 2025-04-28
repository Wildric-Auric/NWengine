#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"
#include "Image.h"
#include "Wave.h"


namespace RandomTst{

RandomMatrix<500, 256, 1> mat;

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
    Image im;
    im.width =  500;
    im.height = 256;
    im.channels = 1;
    im.alpha    = 0;
    im.Alloc();
    for (int i = 0; i < im.height; ++i) {
        for (int j = 0; j < im.width; ++j) {
            im.Write(fVec2(i,j), ImageColor(Normalize<uint32>(mat.Get(i,j), NW_UI32_MAX, 255), 0.0,0.0,1.0));
        }
    }
    im.SaveToFile("Hello.png");
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

