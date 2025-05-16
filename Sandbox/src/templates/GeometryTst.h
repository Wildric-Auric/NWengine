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
#include "LineRenderer.h"
#include "CircleRenderer.h"
#include "Sprite.h"
#include "Geometry.h"

namespace GeometryTst {

struct Data {
	LineRenderer*	lines[8];
	CircleRenderer* pts[10];
	Sprite*			markers[5];
};

v4f exts[10] = {
	// joined
	{-50.0f, -50.0f, 50.0f, 50.0f},
	{-50.0f, 50.0f, 50.0f, -50.0f},
	// perpendicular
	{-150.0f, -20.0f, -100.0f, -20.0f},
	{-125.0f, 50.0f, -125.0f, -50.0f},
	// parallel
	{100.0f, -50.0f, 100.0f, 50.0f},
	{150.0f, -50.0f, 150.0f, 50.0f},
	// Pendicular 2
	{200.0f, 0.0f, 250.0f, 0.0f},
	{225.0f, 50.0f, 225.0f, 0.0f},
	// No intersectoin
	{-300.0f, 0.0f, -250.0f, 100.0f},
	{-275.0f, 0.0f, -275.0f, 40.0f},
};

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
	for(int i = 0; i < 10; i++) {
		data.lines[i] = s.AddObject().AddComponent<LineRenderer>();
		data.lines[i]->SetWidth(2);
		data.lines[i]->SetExt({exts[i].x, exts[i].y}, {exts[i].z, exts[i].w});
	}

	for(int i = 0; i < 10; i++) {
		data.pts[i] = s.AddObject().AddComponent<CircleRenderer>();
		data.pts[i]->SetRadius(5);
	}

	data.pts[0]->SetPosition({0, 110});
	data.pts[1]->SetPosition({200, 110});
	data.pts[2]->SetPosition({170, 200});

	for(int i = 0; i < 5; i++) {
		GameObject& obj = s.AddObject();
		data.markers[i] = obj.AddComponents<Sprite, Transform>();
		data.markers[i]->SetSize({10, 10});
	}

	//-------------
	s.Start();
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() {
	for(int i = 0; i < 5; ++i) {
		Geo::Point pt;
		Geo::Point pt1;
		Geo::Point pt2;
		Geo::Point pt3;
		pt.Set(&data.lines[2 * i]->_start);
		pt1.Set(&data.lines[2 * i]->_end);

		pt2.Set(&data.lines[2 * i + 1]->_start);
		pt3.Set(&data.lines[2 * i + 1]->_end);

		Geo::Segment seg  = Geo::Segment();
		Geo::Segment seg1 = Geo::Segment();
		seg.Set(&pt, &pt1);
		seg1.Set(&pt2, &pt3);

		v2r	 inters;
		bool a = seg.Intersect(seg1, &inters);
		if(a) {
			data.markers[i]->GetGameObject()->GetComponent<Transform>()->position = inters;
		}
	}

	v2f curPos = Inputs::GetMousePosition();
	curPos	   = NWCoordSys::WorldToViewportNonNormalized((NWCoordSys::ScreenNonNormalizedToWorld(curPos)));
	Geo::Point pt, pt1, pt2;
	pt.Set(&data.pts[0]->GetGameObject()->GetComponent<Transform>()->position);
	pt1.Set(&data.pts[1]->GetGameObject()->GetComponent<Transform>()->position);
	pt2.Set(&data.pts[2]->GetGameObject()->GetComponent<Transform>()->position);
	Geo::Point*	  a[] = {&pt, &pt1, &pt2};
	Geo::Triangle tri = Geo::Triangle(a);
	if(tri.IsPtInside(curPos)) {
		data.pts[3]->SetPosition(curPos);
	}

	//-------------------------
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
}; // namespace GeometryTst
