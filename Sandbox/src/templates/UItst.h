#include "CircleRenderer.h"
#include "DefaultAssets.h"
#include "InlineShader.h"
#include "NWengine.h"
#include "Scene.h"
#include "UISys.h"
#include "Text.h"
#include "Renderer.h"
#include "Components.h"
#include "Inputs.h"
#include "UIWindow.h"
#include "NWTime.h"

namespace UITst {

Camera* camC;
void	AddItems(UIWindow& w) {
	   w.GetCursor()->SetLineBreakSize(20);
	   w.GetCursor()->strat = CurAdvanceStrat::BreakOnHorizontalEnd;
	   Sprite*		spr		= w.GetGameObject()->GetComponent<Sprite>();
	   UIItem*		rect;
	   UIItemLabel* label = w.AddItem(UIItemType_Label, -2);
	   label->obj.GetComponent<Text>()->SetContent("Slider: ");
	   rect = w.AddItem(UIItemType_Slider, -1, 2);
	   rect = w.AddItem(UIItemType_Checkbox, -1);
	   for(int i = 0; i < 10; ++i) {
		   rect = w.AddItem(UIItemType_TestZone, -1);
	   }
}

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam		  = s.AddObject();
	GameObject& uwin	  = s.AddObject();
	GameObject& uwin2	  = s.AddObject();
	GameObject& uwin3	  = s.AddObject();
	GameObject& worldObj  = s.AddObject("WorldObj");
	GameObject& worldObj1 = s.AddObject("WorldObj1");
	camC				  = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrthoWithMSAA(900, 500, MSAAValue::NW_MSx8);
	camC->GetFbo()->GenDepthStencilBuffer();
	Renderer::defaultRenderer->SetStretch({1.0, 1.0});

	uwin.AddComponent<UIWindow>()->SetTitle("Hello Window");
	uwin2.AddComponent<UIWindow>();
	uwin2.GetComponent<Transform>()->Translate({-150, 0});
	uwin3.AddComponent<UIWindow>();
	uwin3.GetComponent<Transform>()->Translate({150, 0});

	worldObj.AddComponents<CircleRenderer>()->SetRadius(100);
	worldObj.GetComponent<CircleRenderer>()->SetPosition({200.0, 0.0});
	worldObj.GetComponent<CircleRenderer>()->SetRenderingAA(0.2);
	worldObj1.AddComponents<Sprite, Transform>()->SetSize({200, 200});

	AddItems(*uwin2.GetComponent<UIWindow>());
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
	(*Renderer::defaultRenderer)(true);
	camC->position.x +=
		(Inputs::GetInputKey(NWin::NWIN_KEY_RIGHT, InputKeyEvent::KeyPressed) - Inputs::GetInputKeyPressed(NWin::NWIN_KEY_LEFT)) *
		100.0 * NWTime::GetDeltaTime();

	static float t = 0.0;
	t += NWTime::GetDeltaTime();
	Scene::GetCurrent()->GetGameObject("WorldObj1")->GetComponent<Transform>()->rotation = t;
}

void Run() {
	Context::WINDOW_WIDTH  = 800;
	Context::WINDOW_HEIGHT = 800;

	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}
}; // namespace UITst
