#include "NWengine.h"
#include "Scene.h"
#include "DefaultAssets.h"
#include "LineRenderer.h"


namespace LineTst {

static float t = 0.0;
Transform* tr;
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
    
    GameObject&   l = s.AddObject();
    GameObject&   sq = s.AddObject();
    LineRenderer* lr = l.AddComponent<LineRenderer>();
    lr->SetExt(fVec2(200.0,100.0), fVec2(-400.0,-100.0));

    tr = sq.AddComponent<Transform>();
    Sprite* spr = sq.AddComponent<Sprite>();
    spr->SetTexture(NW_DEFAULT_TEXTURE);
    spr->SetShader("../Sandbox/src/templates/uv.shader");
    spr->SetSize({10,10});
    tr->position = lr->_start;
//    tr->SetRotationAnchor({32,-32});
//    tr->SetScale({3.0,1.0});
//    tr->Translate({0,0});
//
    s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
//    t += 1;
//    tr->SetRotation(t);
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

