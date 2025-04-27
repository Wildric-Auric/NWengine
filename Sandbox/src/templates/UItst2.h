
#include "NWengine.h"
#include "../Scripts.h"	
#include "Scene.h"
#include "RenderingPipeline.h"
#include "UIWindow.h"

RenderingPipeline rpline;
NWPPFX::Bloom bloomTst;
NWPPFX::ColorCorrection cc;
NWPPFX::Tonemapper tm;

Renderer* CRT;
static float t = 0.0;
extern FrameBuffer waterFbo;

namespace UItst2 {

static void UIInit(Scene& s, Camera* camC) {
    GameObject& uwin  = s.AddObject();
    GameObject& uwin2 = s.AddObject();
    GameObject& uwin3 = s.AddObject();
    //GameObject& cam   = s.AddObject();
    //camC = cam.AddComponent<Camera>();
    //camC->Use();
    //camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
    //camC->ChangeOrtho(500,500);
    //Renderer::defaultRenderer->SetStretch({1.0,1.0});

    camC->GetFbo()->GenDepthStencilBuffer();
    uwin.AddComponent<UIWindow>()->SetTitle("Hello Window");
    uwin2.AddComponent<UIWindow>();
    uwin2.GetComponent<Transform>()->Translate({-150,0});
    uwin3.AddComponent<UIWindow>();
    uwin3.GetComponent<Transform>()->Translate({150,0});
}

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& manager = s.AddObject();
	s.Rename("GameManagerObj", &manager);
    manager.AddComponent<Script>()->SetScript<GameManager>();

    CRT = &rpline.AddRenderer();
    CRT->SetShader("../Sandbox/assets/Shaders/MattiasCRT.shader");
	CRT->stretchCoeff.x = 1.3;
	CRT->stretchCoeff.y = 1.3;
	s.Start();
    UIInit(s, s.GetFirstComponent<Camera>());
    bloomTst.luminanceThreshold = 1.0;
	bloomTst.SetUp();

 
    NWPPFX::EffectIO io;
    io.SetInput(bloomTst._fxio.GetOutput());
    cc.SetUp(&io);

    io = {};
    io.SetInput(cc._fxio.GetOutput());
    tm.SetUp(&io);
    

	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
    static bool tmp = 1;

	t += NWTime::GetDeltaTime();
	CRT->componentContainer.GetComponent<Sprite>()->shader->Use();
    CRT->componentContainer.GetComponent<Sprite>()->shader->SetUniform1f("uTime", t);
	CRT->componentContainer.GetComponent<Sprite>()->shader->Unuse();
    Camera::GetActiveCamera()->fbo.Blit(&waterFbo);

    fVec2 winSize;
    fVec2 camSize = Camera::GetActiveCamera()->GetSize(); 
    Context::GetWinDrawAreaSize(&winSize);
    tm._fxio.GetOutput()->SetStretch(winSize/camSize);

    bloomTst.Capture();
    cc.Capture();
    tm.Capture();
    tm.DrawLast();
    //(*CRT)(tm._fxio.GetOutput(), true);


    Camera::GetActiveCamera()->position.x += (Inputs::GetInputKey(NWin::NWIN_KEY_RIGHT, NWin::KeyEventEnum::NWIN_KeyPressed) - 
    Inputs::GetInputKey(NWin::NWIN_KEY_LEFT, NWin::KeyEventEnum::NWIN_KeyPressed)) * 10.0 * NWTime::GetDeltaTime();
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
