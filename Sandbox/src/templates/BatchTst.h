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
#include "Wave.h"
#include "Batch.h"

namespace BatchTst {

GameObject*			go;
SceneObjNode*		actGo;
SceneActiveObjNode* actAGo;

class ObjTstScript : public Scriptable {
  public:
	SCRIPT_CONSTR(ObjTstScript);
	void Update() {}
};

static void Init() {
	// Scene::currentScene->GetGameObject)
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam = s.AddObject("cam");
	GameObject& obj = s.AddObject("obj");
	go				= &obj;
	Camera* camC	= cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);
	s.Start();
	Sprite* spr = obj.AddComponents<Sprite, Transform>();
	actAGo		= s._activeObjs.last;
	actGo		= s._sceneObjs.last;

	GameObject& obj0 = s.AddObject("obj0");
	obj0.AddComponents<Transform, Sprite>()->SetPosition({100.0, 0.0});
	obj0.Get<Sprite>()->SetShader(ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID);
	obj0.Get<Sprite>()->Batch(BatchType::STATIC_BATCH);

	GameObject& obj1 = s.AddObject("obj1");
	obj1.AddComponents<Transform, Sprite>()->SetPosition({-100.0, 0.0});
	obj1.Get<Sprite>()->SetShader(ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID);
	obj1.Get<Sprite>()->Batch(BatchType::DYNAMIC_BATCH);

	spr->SetShader(ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID);
	spr->Batch(BatchType::STATIC_BATCH);
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static float t = 0.0;
static void	 Render() {
	 t += NWTime::GetDeltaTime();
	 SineWave w = SineWave(1.0, 64, 0.0);
	 go->Get<Transform>()->SetPosition({w.Evaluate(t), 0.0});
	 if(Inputs::GetInputOnKeyRelease('S')) {
		 Sprite* spr = go->Get<Sprite>();
		 spr->Batch(BatchType::STATIC_BATCH);
	 }
	 if(Inputs::GetInputOnKeyRelease('D')) {
		 Sprite* spr = go->Get<Sprite>();
		 Batch*	 b	 = (Batch::FindStaticWith(go));
		 if(b)
			 b->InvalidateStaticBatch();
		 spr->UnBatch();
		 spr->Batch(BatchType::DYNAMIC_BATCH);
	 }

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
}; // namespace BatchTst
