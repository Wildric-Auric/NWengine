#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"
#include "Inputs.h"

namespace AudioTst {

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
	// listener
	s.AddObject().AddComponent<AudioListener>();

	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

class ScriptTt : public Scriptable {
  public:
	SCRIPT_CONSTR(ScriptTt);
	void Start() override {
		AudioEmitter* ae = goc->AddComponent<AudioEmitter>();
		ae->SetVolume(2);
		ae->Play();
		printf("\nA\n");
	}
	void Update() override {
		AudioEmitter* ae = goc->AddComponent<AudioEmitter>();
		if(ae->sound == 0)
			return;
		if(ae->sound->HasFinished()) {
			ae->sound->Clean();
			ae->sound = 0;
			Scene::GetCurrent()->DeferredDeleteCurrentGameObject();
			printf("\nHello");
		}
	}
};

static void Render() {
	(*Renderer::defaultRenderer)(true);
	if(Inputs::GetInputKey('A', InputKeyEvent::OnKeyRelease)) {
		GameObject&	  obj = Scene::GetCurrent()->AddObject();
		AudioEmitter* ae  = obj.AddComponent<AudioEmitter>();
		ae->SetSound("C:/Programming/Games/TopDownShooter/example/assets/Sounds/wilhelm.wav");
		Script* s = obj.AddComponent<Script>();
		s->SetScript<ScriptTt>();
		s->Start();
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
}; // namespace AudioTst
