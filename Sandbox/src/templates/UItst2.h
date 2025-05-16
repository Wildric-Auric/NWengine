
#include "../Scripts.h"
#include "NWengine.h"
#include "RenderingPipeline.h"
#include "Scene.h"
#include "UIWindow.h"
#include "NWTime.h"
#include "Inputs.h"
#include "NWin/keys.h"

RenderingPipeline		rpline2;
NWPPFX::Bloom			bloomTst2;
NWPPFX::ColorCorrection cc2;
NWPPFX::Tonemapper		tm2;

Renderer*		   CRT2;
static float	   t2 = 0.0;
extern FrameBuffer waterFbo;

namespace UItst2 {

static void UIInit(Scene& s, Camera* camC) {
	GameObject& uwin  = s.AddObject();
	GameObject& uwin2 = s.AddObject();
	GameObject& uwin3 = s.AddObject();
	// GameObject& cam   = s.AddObject();
	// camC = cam.AddComponent<Camera>();
	// camC->Use();
	// camC->SetClearColor(fVec4(0.2,0.0,1.0,1.0));
	// camC->ChangeOrtho(500,500);
	// Renderer::defaultRenderer->SetStretch({1.0,1.0});

	camC->GetFbo()->GenDepthStencilBuffer();
	uwin.AddComponent<UIWindow>()->SetTitle("Hello Window");
	uwin2.AddComponent<UIWindow>();
	uwin2.GetComponent<Transform>()->Translate({-150, 0});
	uwin3.AddComponent<UIWindow>();
	uwin3.GetComponent<Transform>()->Translate({150, 0});
}

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& manager = s.AddObject();
	s.Rename("GameManagerObj", &manager);
	manager.AddComponent<Script>()->SetScript<GameManager>();

	CRT2 = &rpline2.AddRenderer();
	CRT2->SetShader("../Sandbox/assets/Shaders/MattiasCRT.shader");
	CRT2->stretchCoeff.x = 1.3;
	CRT2->stretchCoeff.y = 1.3;
	s.Start();
	UIInit(s, s.GetFirstComponent<Camera>());
	bloomTst2.luminanceThreshold = 1.0;
	bloomTst2.SetUp();

	NWPPFX::EffectIO io;
	io.SetInput(bloomTst2._fxio.GetOutput());
	cc2.SetUp(&io);

	io = {};
	io.SetInput(cc2._fxio.GetOutput());
	tm2.SetUp(&io);

	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
	static bool tm2p = 1;

	t2 += NWTime::GetDeltaTime();
	CRT2->componentContainer.GetComponent<Sprite>()->shader->Use();
	CRT2->componentContainer.GetComponent<Sprite>()->shader->SetUniform1f("uTime", t2);
	CRT2->componentContainer.GetComponent<Sprite>()->shader->Unuse();
	Camera::GetActiveCamera()->fbo.Blit(&waterFbo);

	fVec2 winSize;
	fVec2 camSize = Camera::GetActiveCamera()->GetSize();
	Context::GetWinDrawAreaSize(&winSize);
	tm2._fxio.GetOutput()->SetStretch(winSize / camSize);

	bloomTst2.Capture();
	cc2.Capture();
	tm2.Capture();
	tm2.DrawLast();
	//(*CRT2)(tm2._fxio.GetOutput(), true);

	Camera::GetActiveCamera()->position.x += (Inputs::GetInputKey(NWin::NWIN_KEY_RIGHT, InputKeyEvent::KeyPressed) -
											  Inputs::GetInputKey(NWin::NWIN_KEY_LEFT, InputKeyEvent::KeyPressed)) *
											 10.0 * NWTime::GetDeltaTime();
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
}; // namespace UItst2
