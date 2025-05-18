#include "CircleRenderer.h"
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
UIItemLabel* label;
UIItem*		 slider;
Camera*		 camC;

static Renderer	  rnd;
static Renderer	  compositor;
static UIManager* man;
void			  AddItems(UIWindow& w) {
	 w.GetCursor()->SetLineBreakSize(20);
	 w.GetCursor()->strat = CurAdvanceStrat::BreakOnHorizontalEnd;
	 Sprite* spr		  = w.GetGameObject()->GetComponent<Sprite>();
	 UIItem* rect;
	 label = w.AddItem(UIItemType_Label, -2, 1);
	 UISetLabel(label, "Slider Value: ");
	 slider						   = w.AddItem(UIItemType_Slider, -1, 2);
	 UIGetSliderData(slider)->minn = -100.0f;
	 UIGetSliderData(slider)->maxx = 100.0f;
	 rect						   = w.AddItem(UIItemType_Checkbox, -1, 1);
	 for(int i = 0; i < 10; ++i) {
		 rect = w.AddItem(UIItemType_TestZone, -1);
	 }
}

void SetWin(UIWindow* win) {
	Camera*	   cam	= Camera::ActiveCamera;
	Transform* tr	= win->attachedObject->Get<Transform>();
	UIWindow*  uwin = win->attachedObject->Get<UIWindow>();
	v2f		   s;
	s = cam->GetSize();
	s.x *= 0.15;
	uwin->SetTitle("Docked Win");
	uwin->SetSize(s);
	uwin->SetPosition({-cam->GetSize().x * 0.5f + s.x * 0.5f, 0.0});
	uwin->prop &= ~Window_Prop_ResizableXL;
	uwin->prop &= ~Window_Prop_ResizableYU;
	uwin->prop &= ~Window_Prop_Movable;
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

	uwin.AddComponent<UIWindow>()->SetTitle("Hello Window");
	uwin2.AddComponent<UIWindow>();
	uwin2.GetComponent<Transform>()->Translate({-150, 0});
	uwin3.AddComponent<UIWindow>();
	uwin3.GetComponent<Transform>()->Translate({150, 0});

	worldObj.AddComponents<CircleRenderer>()->SetRadius(100);
	worldObj.GetComponent<CircleRenderer>()->SetPosition({200.0, 0.0});
	worldObj.GetComponent<CircleRenderer>()->SetRenderingAA(0.2);
	worldObj1.AddComponents<Sprite, Transform>()->SetSize({200, 200});

	UIWindow*  w = uwin2.Get<UIWindow>();
	UIManager& m = uwin2.Add<UIManager>();
	man			 = &m;
	SetWin(w);
	AddItems(*uwin2.GetComponent<UIWindow>());
	
	s.Start();
	w->attachedUIManager = &m;
	uwin.Get<UIWindow>()->attachedUIManager = &m;
	uwin3.Get<UIWindow>()->attachedUIManager = &m;

	//  printf("NW_VERSION: %s\n", NWengineGetVersionString());
	compositor.SetUp();
	rnd.SetUp();
	compositor.Use();

	Renderer::currentRenderer->SetStretch({1.0, 1.0});
}

static void Render() {
	rnd(false);
	rnd.Composite(&man->rnd);
	compositor(&rnd, true);
//	(*Renderer::currentRenderer)(true);

	camC->position.x +=
		(Inputs::GetInputKey(NWin::NWIN_KEY_RIGHT, InputKeyEvent::KeyPressed) - Inputs::GetInputKeyPressed(NWin::NWIN_KEY_LEFT)) *
		100.0 * NWTime::GetDeltaTime();

	static float t = 0.0;
	t += NWTime::GetDeltaTime();
	Scene::GetCurrent()->GetGameObject("WorldObj1")->GetComponent<Transform>()->rotation = t;
	UISetLabel(label, (std::string("Slider Value: ") + std::to_string(UIGetSliderValue(slider)).substr(0, 5)).c_str());
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
