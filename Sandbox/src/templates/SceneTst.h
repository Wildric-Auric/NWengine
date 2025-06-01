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

namespace SceneTst {

GameObject*			go;
SceneObjNode*		actGo;
SceneActiveObjNode* actAGo;

class ObjTstScript : public Scriptable {
  public:
	SCRIPT_CONSTR(ObjTstScript);
	float t = 0.0;
	void  Update() override {
		 t += NWTime::GetDeltaTime();
		 SineWave w = SineWave(1.0, 64, 0.0);
		 go->Get<Transform>()->SetPosition({w.Evaluate(t), 0.0});
	}
};

static void Init() {
	// Scene::currentScene->GetGameObject)
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam = s.AddObject();
	GameObject& obj = s.AddObject();
	go				= &obj;
	Camera* camC	= cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);
	s.Start();
	Sprite* spr		  = obj.AddComponents<Sprite, Transform>();
	actAGo			  = s._activeObjs.last;
	actGo			  = s._sceneObjs.last;
	ObjTstScript* scr = obj.AddComponent<Script>()->SetScript<ObjTstScript>();

	GameObject& obj0 = s.AddObject();
	GameObject& obj1 = s.AddObject();
	for(int i = 0; i < 50000; ++i) {
		GameObject& obj2 = s.DirectAddObject();
		obj2.AddComponents<Transform, Sprite>()->SetPosition({100.0, 0.0});
	}
	for(int i = 0; i < 50000; ++i) {
		s.DeleteLastObject();
	}
	obj0.AddComponents<Transform, Sprite>()->SetPosition({100.0, 0.0});
	obj1.AddComponents<Transform, Sprite>()->SetPosition({-100.0, 0.0});

	// spr->SetShader(ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID);
	// spr->Batch(BatchType::STATIC_BATCH);
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static float t = 0.0;
static void	 Render() {
	 if(Inputs::GetInputOnKeyRelease('A')) {
		 // actAGo->cont->cont.Get<Sprite>()->StopRendering();
		 Scene::currentScene->DisableObject(actAGo);
		 // Scene::currentScene->ForceRenderStop();
	 }
	 if(Inputs::GetInputOnKeyRelease('Z')) {
		 actAGo = Scene::currentScene->EnableObject(actGo);
		 // actAGo->cont->cont.Get<Sprite>()->Render();
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
}; // namespace SceneTst
