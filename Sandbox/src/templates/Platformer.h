
#include "../platformer/PlayerController.h"
#include "NWengine.h"
#include "RenderingPipeline.h"
#include "Scene.h"
#include "Collider.h"
#include "Sprite.h"
#include "Transform.h"
#include "NWTime.h"

namespace Platformer {
static void Init() {
	Context::SetTitle("Platformer");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();

    GameObject& cam = s.AddObject<Transform, Camera>("camObj");
    GameObject& gnd = s.AddObject<Transform, Sprite, Collider>("gndObj");
    GameObject& ply = s.AddObject<Transform, Sprite, Collider>("plyrObj");

    Camera& camComp = *cam.Get<Camera>();
    camComp.Use();
    camComp.ChangeOrtho(1280, 720);
    camComp.SetClearColor(v4f(0.18,0.0,0.1,1.0));

    Sprite& sg = *gnd.Get<Sprite>(); 
    Sprite& sp = *ply.Get<Sprite>();
    Transform& g = *gnd.Get<Transform>(); 
    Transform& p = *ply.Get<Transform>();
    ply.Add<Script>().SetScript<PlayerController>();
    ply.Get<Collider>()->SetEdgesSprite();
    gnd.Get<Collider>()->SetEdgesSprite();

    sg.SetSize({720,32});
    sp.SetSize({32,64});
    g.SetPosition({0, -64});

    s.Start();
}

static void Render() {
	static bool tmp = 1;
    (*Renderer::currentRenderer)(1);
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
};
