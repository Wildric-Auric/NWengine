#include "SceneSet.h"
#include "Scene.h"
#include "PrimManager.h"

static void SetBg() {
    Scene& s = *Scene::GetCurrent();
    Camera& cam = *Camera::GetActiveCamera();
	GameObject& bg		= s.AddObject();
	bg.AddComponent<Transform>();

	Sprite* spr = bg.AddComponent<Sprite>();
	spr->SetShader("../Sandbox/src/templates/Blueprint.shader");
	spr->SetSize(cam.GetSize());
	spr->GetShader()->Use();
	spr->GetShader()->SetVector2("uResolution", spr->container.width, spr->container.height);
	spr->GetShader()->SetVector2("uCell", 50.0f, 50.0f);
	spr->sortingLayer = NW_I32_MIN;
}

static void SetCam() {
    Scene& s = *Scene::GetCurrent();
	GameObject& cam	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(1080, 720);
}

void SceneSet::Start() { 
    Scene& s = *Scene::GetCurrent();
    SetCam();
    SetBg();

    s.AddObject().AddComponent<Script>()->SetScript<PrimManager>();
}
