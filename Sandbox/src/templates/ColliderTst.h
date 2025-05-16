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
#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"

namespace ColliderTst {

GameObject* col0;
GameObject* col1;

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);

	col0 = &s.AddObject();
	col0->AddComponent<Transform>();
	col0->AddComponent<Sprite>()->SetSize({200, 200});
	col0->AddComponent<Collider>()->SetEdgesSprite();

	col1 = &s.AddObject();
	col1->AddComponent<Transform>()->Translate({300, 0});
	col1->AddComponent<Sprite>()->SetSize({150, 200});
	col1->AddComponent<Collider>()->SetEdgesSprite();

	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
	(*Renderer::defaultRenderer)(true);
	Transform* tr = col1->GetComponent<Transform>();
	tr->Translate(fVec2((Inputs::right - Inputs::left) * NWTime::GetDeltaTime() * 100,
						(Inputs::up - Inputs::down) * NWTime::GetDeltaTime() * 100));
	if(col0->GetComponent<Collider>()->isColliding(col1->GetComponent<Collider>())) {
		printf("hek\n");
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
}; // namespace ColliderTst
