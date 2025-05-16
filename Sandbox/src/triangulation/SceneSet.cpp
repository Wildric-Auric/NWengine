#include "SceneSet.h"
#include "Scene.h"
#include "PrimManager.h"
#include "Utilities.h"
#include "DefaultAssets.h"
#include "CircleRenderer.h"

static void SetBg() {
	Scene&		s	= *Scene::GetCurrent();
	Camera&		cam = *Camera::GetActiveCamera();
	GameObject& bg	= s.AddObject("blueprint");
	bg.AddComponent<Transform>();

	Sprite* spr = bg.AddComponent<Sprite>();
	spr->SetShader("../Sandbox/src/templates/Blueprint.shader");
	spr->SetSize(cam.GetSize());
	spr->GetShader()->Use();
	spr->GetShader()->SetVector2("uResolution", spr->container.width, spr->container.height);
	spr->GetShader()->SetVector2("uCell", 50.0f, 50.0f);
	spr->SetSortingLayer(NW_I32_MAX);
}

static void SetCam() {
	Scene&		s	 = *Scene::GetCurrent();
	GameObject& cam	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(1080, 720);
}

static void SetText() {
	Scene&		s	= *Scene::GetCurrent();
	GameObject& obj = s.AddObject<Sprite, Transform, Text>();
	Text*		te	= obj.GetComponent<Text>();
	std::string fdir;
	GetSystemFontDir(&fdir);
	fdir += "Arial.ttf";
	te->isBatched  = 1;
	te->layerOrder = 0;
	te->SetShader(NW_DEFAULT_SHADER_TEXT_BATCHED);
	te->colors = v4f(1.0, 0.0, 0.0, 1.0);
	te->SetFont({fdir.c_str(), 15});
	te->SetContent("'T': Ear Clipping Triangulation\n"
				   "'S': Delaunay Triangulation\n"
				   "'R': reset\n"
				   "'A': circumcircle of a triangle.");
	te->UpdateGlyphs();
	v2f camS = Camera::ActiveCamera->GetSize();
	v2f tep	 = te->GetPosition();
	v2f ts	 = te->GetSize();
	te->SetPosition(tep + camS * 0.5 - ts * 0.5);
	te->UpdateGlyphs();
	obj.GetComponent<Sprite>()->SetSize(ts);
	obj.GetComponent<Transform>()->SetPosition(te->_bb.center);

	GameObject& obj2 = s.AddObject<CircleRenderer>("disc");
	Sprite*		spr	 = obj2.GetComponent<Sprite>();
	spr->SetShader("../Sandbox/src/triangulation/disc.shader");
	spr->StopRendering();
}

void SceneSet::Start() {
	Scene& s = *Scene::GetCurrent();
	SetCam();
	SetBg();
	SetText();
	s.AddObject().AddComponent<Script>()->SetScript<PrimManager>();
}
