#include "NWengine.h"
#include "RuntimeManager.h"
#include "Scene.h"
#include "nwin/window.h"
#include "nwin/gl_context.h"



void Init() {
	//The following line is a residue of the dark time when the core wasn't separated from the Editor; it is deprecated and will be removed soon
	RuntimeManager::switchMode(EngineMode::PLAY_MODE);
	//Creates new scene
	Scene& scene = Scene::CreateNew("new scene");
	scene.MakeCurrent();

	//Adds camera to the scene
	GameObject& tmp   =  scene.AddObject();
	scene.Rename("CameraObj", &tmp);


	GameObject& box = scene.AddObject();
	scene.Rename("BoxObj", &box);
	

	Camera&     cam   = *tmp.AddComponent<Camera>();
	cam.clearColor = fVec3(0.3, 0.35, 0.4);
	cam.ChangeOrtho(300,300);

	Sprite&  sprite = *box.AddComponent<Sprite>();
	box.AddComponent<Transform>();
	sprite.SetTexture("Ressources\\Images\\DefaultBox10x10.png");
	cam.Use();


	scene.Start();
}

void Render() {
	Renderer& renderer = (*Renderer::currentRenderer);
	renderer(true);
}


struct phyData {
	float lastAcc = 0;
	float lastVel = 0;
	float vel     = 0;
	float acc     = 0;
	float mass    = 10.0;
	float pos     = 0;
	float lastPos = 0;
};

static phyData data{};
static float   g  = 10;

void Update() {
	
	float dt = NWTime::GetDeltaTime();
	NWin::Window& w= *(NWin::Window*)(Context::window);
	NWin::GlContext::setCurCtxVSync(1);
	auto kb = w._getKeyboard();
	Transform* trans = Scene::currentScene->GetGameObject("BoxObj")->GetComponent<Transform>();
	float F        = Inputs::up * 6000.0f;
	//Compute acceleration
	data.lastAcc = data.acc;
	data.acc	 = -300.0 + F/data.mass; //Newton Law of motion
	//Compute speed.
	data.lastVel = data.vel;
	data.vel	 = dt * data.acc + data.lastVel;
	//Compute position;
	data.lastPos = data.pos;
	data.pos     = dt * data.vel + data.lastPos;
	//----------------------------


	
	trans->position.y = data.pos;
	if (trans->position.y < 0.0) {
		trans->position.y = 0.0;
		data.vel          = 0.0;
		data.acc          = 0.0;
		data.pos          = 0.0;
	}

}

int main() {
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Update);

	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
	return 0;
}

