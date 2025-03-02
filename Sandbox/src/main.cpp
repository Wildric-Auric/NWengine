#include "NWengine.h"
#include "Scripts.h"	
#include "Scene.h"
#include "RenderingPipeline.h"

RenderingPipeline rpline;

NWPPFX::Bloom bloomTst;
NWPPFX::ColorCorrection cc;
NWPPFX::Tonemapper tm;


Renderer* CRT;
void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& manager = s.AddObject();
	s.Rename("GameManagerObj", &manager);
	manager.AddComponent<Script>()->SetScript<GameManager>();
	manager.AddComponent<MultiAudioEmitter>()->AddEmitter();
	manager.DeleteComponent<MultiAudioEmitter>();

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

static float t = 0.0;

extern FrameBuffer waterFbo;

void Render() {
	t += NWTime::GetDeltaTime();
	CRT->componentContainer.GetComponent<Sprite>()->shader->Use();
    CRT->componentContainer.GetComponent<Sprite>()->shader->SetUniform1f("uTime", t);
	CRT->componentContainer.GetComponent<Sprite>()->shader->Unuse();
   //rpline.Capture();
   //rpline.DrawLast();
    Camera::GetActiveCamera()->fbo.Blit(&waterFbo);

    fVec2 winSize;
    fVec2 camSize = fVec2(720.0f,480.0f); //tm._fxio.GetOutput()->GetCamera()->size;
    Context::GetWinDrawAreaSize(&winSize);
    tm._fxio.GetOutput()->SetStretch(winSize/camSize);

    bloomTst.Capture();
    cc.Capture();
    tm.Capture();

    tm.DrawLast();
    //(*CRT)(tm._fxio.GetOutput(), true);
}

int main() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
	return 0;
}

