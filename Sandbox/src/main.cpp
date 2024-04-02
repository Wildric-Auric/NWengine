#include "NWengine.h"
#include "RuntimeManager.h"
#include "ScriptManager.h"	
#include "Scene.h"
#include "nwin/window.h"
#include "nwin/gl_context.h"

#include "Rocket.h"

void Rocket::Start() {
	
}

void Rocket::Update() {
	float dt = NWTime::GetDeltaTime();
	NWin::Window& w = *(NWin::Window*)(Context::window);
	NWin::GlContext::setCurCtxVSync(1);
	auto kb = w._getKeyboard();
	Transform* trans = Scene::currentScene->GetGameObject("BoxObj")->GetComponent<Transform>();
	float F = Inputs::up * 6000.0f;
	//Compute acceleration
	data.lastAcc = data.acc;
	data.acc = -300.0 + F / data.mass; //Newton Law of motion
	//Compute speed.
	data.lastVel = data.vel;
	data.vel = dt * data.acc + data.lastVel;
	//Compute position;
	data.lastPos = data.pos;
	data.pos = dt * data.vel + data.lastPos;
	//----------------------------
	trans->position.y = data.pos;
	if (trans->position.y < 0.0) {
		trans->position.y = 0.0;
		data.vel = 0.0;
		data.acc = 0.0;
		data.pos = 0.0;
	}
	if (Inputs::down) {
		goc->GetComponent<Sprite>()->UnBatch();
		goc->GetComponent<Sprite>()->SetShader("Ressources/Shaders/Textured.shader");
	}
}

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
	cam.ChangeOrtho(1080, 720);

	Sprite&  sprite = *box.AddComponent<Sprite>();
	box.AddComponent<Transform>();
	sprite.SetTexture("Ressources\\Images\\DefaultBox10x10.png");
	sprite.Batch();
	sprite.SetShader("Ressources/Shaders/TexturedBatched.shader");
	box.AddComponent<Script>()->script = new Rocket(&box);
	auto t = box.AddComponent<Text>();
	auto e = box.AddComponent<AudioEmitter>();
	auto l = box.AddComponent<AudioListener>();

	//t->shader = "Ressources/Shaders/Textured.shader";
	t->text   = "69";
	//t->SetFont(FONT_DEFAULT);
	//t->UpdateGlyphs();

	//e->SetSound("Ressources/Sounds/Wallpaper.wav");
	//e->sound->Play();

	cam.Use();
	scene.Start();
}

void Render() {
	Renderer& renderer = (*Renderer::currentRenderer);
	renderer(true);
}

static float   g  = 10;

void Update() {

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

