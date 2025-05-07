#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"
#include "LineRenderer.h"
#include "CircleRenderer.h"
#include "Sprite.h"

namespace GeometryTst {

struct Data {
	LineRenderer*	lines[10];
	CircleRenderer* pts[10];
	Sprite*			markers[10];
};

v4f exts[8] = {
	// 0: diagonal down-right (slope -1)
	{0.0f, 0.0f, 56.0f, -56.0f},
	// 1: diagonal up-right (slope ≈ 0.653)
	{-53.0f, -50.0f, 100.0f, 50.0f},
	// 2: horizontal at y=10
	{10.0f, 10.0f, 200.0f, 10.0f},
	// 3: horizontal at y=10, overlapping 2
	{50.0f, 10.0f, 150.0f, 10.0f},
	// 4: horizontal at y=10, disjoint from 2
	{300.0f, 10.0f, 400.0f, 10.0f},
	// 5: vertical at x=400
	{400.0f, 0.0f, 400.0f, 800.0f},
	// 6: horizontal at y=400, intersects 5
	{0.0f, 400.0f, 800.0f, 400.0f},
	// 7: diagonal slope +1, perpendicular to 0 but far apart
	{500.0f, 500.0f, 600.0f, 600.0f}};

Data data;

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(800, 800);
	//--------------
	for(int i = 0; i < 8; i++) {
		data.lines[i] = s.AddObject().AddComponent<LineRenderer>();
		data.lines[i]->SetWidth(2);
		data.lines[i]->SetExt({exts[i].x, exts[i].y}, {exts[i].z, exts[i].w});
	}

//	for(int i = 0; i < 10; i++) {
//		data.pts[i] = s.AddObject().AddComponent<CircleRenderer>();
//		data.pts[i]->SetRadius(5);
//	}
//
//	for(int i = 0; i < 10; i++) {
//		GameObject& obj = s.AddObject();
//		obj.AddComponent<Transform>();
//		data.markers[i] = obj.AddComponent<Sprite>();
//		data.markers[i]->SetSize({10, 10});
//	}

	//-------------
	s.Start();
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
}; // namespace GeometryTst
