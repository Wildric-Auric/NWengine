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

namespace BatchTst {

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam	 = s.AddObject();
	GameObject& obj	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);
	s.Start();
	Sprite* spr = obj.AddComponents<Sprite, Transform>();
	spr->SetShader(ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID);
	spr->Batch(BatchType::STATIC_BATCH);
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() { (*Renderer::defaultRenderer)(true); }

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}
}; // namespace BatchTst
