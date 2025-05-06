#include "CircleRenderer.h"
#include "DefaultAssets.h"
#include "InlineShader.h"
#include "NWengine.h"
#include "Scene.h"
#include "UISys.h"
#include "UIWindow.h"

namespace UITst {

Camera* camC;

void AddItems(UIWindow& w) {
	w.GetCursor()->SetLineBreakSize(20);
	w.GetCursor()->strat = CurAdvanceStrat::BreakOnHorizontalEnd;
	Sprite* spr			 = w.GetGameObject()->GetComponent<Sprite>();
	UIItem* rect		 = w.AddItem(UIItemType::TEST_ZONE, -1);
	Sprite* spr2		 = rect->obj.AddComponent<Sprite>();
	rect->obj.AddComponent<Transform>();
	InlineShader colorShader;
	colorShader.SetFragOut("vec4(1.0,0.0,1.0,1.0)");
	colorShader.Generate();
	spr2->SetShader(colorShader.GetShader());
	spr2->sortingLayer = spr->sortingLayer - 1;

	rect = w.AddItem(UIItemType::TEST_ZONE, -1);
	spr2 = rect->obj.AddComponent<Sprite>();
	rect->obj.AddComponent<Transform>();
	spr2->SetShader(colorShader.GetShader());
	spr2->sortingLayer = spr->sortingLayer - 1;
	spr2->SetSize({30, 20});

	for(int i = 0; i < 10; ++i) {
		rect = w.AddItem(UIItemType::TEST_ZONE, -1);
		spr2 = rect->obj.AddComponent<Sprite>();
		rect->obj.AddComponent<Transform>();
		spr2->SetShader(colorShader.GetShader());
		spr2->sortingLayer = spr->sortingLayer - 1;
	}
}

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam		 = s.AddObject();
	GameObject& uwin	 = s.AddObject();
	GameObject& uwin2	 = s.AddObject();
	GameObject& uwin3	 = s.AddObject();
	GameObject& worldObj = s.AddObject();
	camC				 = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(500, 500);
	camC->GetFbo()->GenDepthStencilBuffer();
	Renderer::defaultRenderer->SetStretch({1.5, 1.5});

	uwin.AddComponent<UIWindow>()->SetTitle("Hello Window");
	uwin2.AddComponent<UIWindow>();
	uwin2.GetComponent<Transform>()->Translate({-150, 0});
	uwin3.AddComponent<UIWindow>();
	uwin3.GetComponent<Transform>()->Translate({150, 0});

	worldObj.AddComponent<CircleRenderer>();

	AddItems(*uwin2.GetComponent<UIWindow>());
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
	(*Renderer::defaultRenderer)(true);
	camC->position.x +=
		(Inputs::GetInputKey(NWin::NWIN_KEY_RIGHT, InputKeyEvent::KeyPressed) - Inputs::GetInputKeyPressed(NWin::NWIN_KEY_LEFT)) *
		100.0 * NWTime::GetDeltaTime();
}

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	Context::WINDOW_WIDTH		= 800;
	Context::WINDOW_HEIGHT		= 800;

	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}
}; // namespace UITst
