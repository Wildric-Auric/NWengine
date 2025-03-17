
#include "NWengine.h"
#include "../Scripts.h"	
#include "Scene.h"
#include "RenderingPipeline.h"

RenderingPipeline rpline;
NWPPFX::Bloom bloomTst;
NWPPFX::ColorCorrection cc;
NWPPFX::Tonemapper tm;

Renderer* CRT;
static float t = 0.0;
extern FrameBuffer waterFbo;

namespace Sandbox {

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

    if (tmp && t > 3.0) {
        Image im;
        tm.GetOutput()->GetCamera()->GetFbo()->CopyFramebufferToCPU(&im);
        im.SaveToFile("C:/Users/HP/source/repos/Wildric-Auric/NWengine/b.png");
        im.Clean();
        tmp = 0;
    }
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
