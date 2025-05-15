#include "PrimManager.h"
#include "Scene.h"
#include "Transform.h"
#include "LineRenderer.h"
#include "CircleRenderer.h"
#include "Inputs.h"
#include "CoordSys.h"
#include "Maths.h"
#include "keyboard.h"
#include "NWTime.h"
#include "Geometry.h"

void PrimManager::Start() {
	//    TriPoint& pt = AddPoint();
	//    TriPoint& pt1 = AddPoint();
	//    pt.SetUp(v2f(0.0,0.0));
	//    pt1.SetUp(v2f(100.0,0.0));
	//    TriLine& l = AddLine();
	//    l.SetUp(&pt, &pt1);
	edges.SetUp(sizeof(TriEdge), 0xFF);
}

static TriPoint* last;
static TriEdge*	 edge;
static TriEdge*	 fedge;

TriPoint* PrimManager::FindPt(const v2r& p) {
	for(TriPoint& pt : pts) {
		if(pt.Get() == p)
			return &pt;
	}
	return 0;
}

void PrimManager::_TestEdges() {
	if(!Inputs::GetInputOnKeyRelease('B') && !Inputs::GetInputKeyPressed('X') && !Inputs::GetInputOnKeyRelease('T') &&
	   !Inputs::GetInputOnKeyRelease('S') && !Inputs::GetInputOnKeyRelease('A'))
		return;
	if(!lines.size())
		return;
	if(Inputs::GetInputKeyPressed('C')) {
		addr e	 = edges.GetFirst();
		int	 sss = 0;
		while(e) {
			if(sss == int(tmpf) - 1) {
				TriEdge* ppp = CAST(TriEdge*, edges.GetContent(e));
				ppp->line->obj->GetComponent<LineRenderer>()->SetWidth(ppp->line->_m->lineWidth);
			}
			if(sss == int(tmpf)) {
				TriEdge* ppp = CAST(TriEdge*, edges.GetContent(e));
				ppp->line->obj->GetComponent<LineRenderer>()->SetWidth(0);
			}
			e = edges.GetNext(e);
			sss++;
		}
		tmpf += NWTime::GetDeltaTime();
		if(tmpf >= edges._size) {
			tmpf		 = 0.0;
			TriEdge* ppp = CAST(TriEdge*, edges.GetContent(edges.GetLast()));
			ppp->line->obj->GetComponent<LineRenderer>()->SetWidth(ppp->line->_m->lineWidth);
		}
	}
	//----------

	Geo::Polygon poly;
	struct Tmp {
		TriPoint*					  ptr;
		int							  i = 0;
		std::list<TriPoint>::iterator iter;
	};
	int	 s	  = pts.size() - 1;
	auto beg0 = pts.begin();
	auto end0 = --pts.end();
	int	 beg  = 0;
	int	 end  = pts.size() - 1;
	poly.SetUp((void*)beg, (void*)end,
			   [](Geo::Polygon* p, void* e) -> void* {
				   int c = (int)e;
				   if(c == (int)p->_last)
					   return p->_first;
				   return (void*)(c + 1);
			   },
			   [](Geo::Polygon* p, void* e) -> void* {
				   int c = (int)e;
				   if(c == (int)p->_first)
					   return p->_last;
				   return (void*)(c - 1);
			   },
			   [](Geo::Polygon* p, void* e) -> v2r* {
				   int	c	= (int)e;
				   auto itr = ((std::list<TriPoint>*)p->_data)->begin();
				   std::advance(itr, c);
				   return itr->GetRef();
			   });
	poly.SetData(&pts);

	if(Inputs::GetInputOnKeyRelease('T')) {
		Triangulate(poly);
		return;
	}

	if(Inputs::GetInputOnKeyRelease('S')) {
		Geo::DelaunayTriangulator ttr;
		Geo::TriangleData		  trid;
		Geo::Triangle			  tri;
		ttr.Alloc(&poly, pts.size());
		ttr.ComputeSuperTriangle(&trid);
		ttr.Process();
		for(int i = 0; i < ttr.triNum; ++i) {
			TriPoint* pt  = FindPt(ttr._tris[i * 3]);
			TriPoint* pt1 = FindPt(ttr._tris[i * 3 + 1]);
			TriPoint* pt2 = FindPt(ttr._tris[i * 3 + 2]);
#define CHK(pt) pt->Get() == trid.pts[0] || pt->Get() == trid.pts[1] || pt->Get() == trid.pts[2]
			if(CHK(pt) || CHK(pt1) || CHK(pt2))
				continue;
			AddLine().SetUp(pt, pt1);
			AddLine().SetUp(pt, pt2);
			AddLine().SetUp(pt2, pt1);
		}

		ttr.Clean();
		//		Geo::Point	pts[3];
		//		Geo::Point* ptr[3];
		//		pts[0].Set(&trid.pts[0]);
		//		pts[1].Set(&trid.pts[1]);
		//		pts[2].Set(&trid.pts[2]);
		//		ptr[0] = &pts[0];
		//		ptr[1] = &pts[1];
		//		ptr[2] = &pts[2];
		//		tri.Set(ptr);
		//		GameObject* obj = Scene::currentScene->GetGameObject("disc");
		//		if(obj) {
		//			v2r cntr = tri.CalcCircCenter();
		//			obj->GetComponent<CircleRenderer>()->SetPosition(cntr);
		//			obj->GetComponent<CircleRenderer>()->SetRadius((cntr - *tri.GetPt(0)).magnitude());
		//			obj->GetComponent<Sprite>()->Render();
		//		}
		return;
	}

	if(Inputs::GetInputOnKeyRelease('A')) {
		Geo::DelaunayTriangulator ttr;
		Geo::TriangleData		  trid;
		Geo::Triangle			  tri;

		trid.pts[0] = *(this->pts.front().GetRef());
		trid.pts[1] = *((++this->pts.begin())->GetRef());
		trid.pts[2] = *((++(++this->pts.begin()))->GetRef());

		printf("%d\n", this->pts.size()); // TODO::Del
		AddPoint().SetUp(trid.pts[0]);
		AddPoint().SetUp(trid.pts[1]);
		AddPoint().SetUp(trid.pts[2]);
		Geo::Point	pts[3];
		Geo::Point* ptr[3];
		pts[0].Set(&trid.pts[0]);
		pts[1].Set(&trid.pts[1]);
		pts[2].Set(&trid.pts[2]);
		ptr[0] = &pts[0];
		ptr[1] = &pts[1];
		ptr[2] = &pts[2];
		tri.Set(ptr);
		GameObject* obj = Scene::currentScene->GetGameObject("disc");
		if(obj) {
			v2r cntr = tri.CalcCircCenter();
			obj->GetComponent<CircleRenderer>()->SetPosition(cntr);
			obj->GetComponent<CircleRenderer>()->SetRadius((cntr - *tri.GetPt(0)).magnitude());
			obj->GetComponent<Sprite>()->Render();
		}
	}

	//---------
	if(Inputs::GetInputOnKeyRelease('B')) {
		printf("%s\n", "PolyTest");
		printf("Is Convex: %d\n", poly.IsConvex());
	}

	fVec2 cur = Inputs::GetMousePosition();
	cur		  = NWCoordSys::WorldToViewportNonNormalized((NWCoordSys::ScreenNonNormalizedToWorld(cur)));
	printf("Winding order: %s \n",
		   (poly.CalcOrientation() == Geo::PolyOrientation::CCW) ? "Geo::PolyOrientation::CCW" : "Geo::PolyOrientation::CW");
}

void PrimManager::MakeLineOnClick() {
	constexpr float tol	   = 0.5;
	bool			click  = Inputs::GetInputOnKeyRelease(NWin::NWIN_KEY_LBUTTON);
	bool			rclick = Inputs::GetInputOnKeyRelease(NWin::NWIN_KEY_RBUTTON);
	fVec2			cur	   = Inputs::GetMousePosition();
	cur					   = NWCoordSys::WorldToViewportNonNormalized((NWCoordSys::ScreenNonNormalizedToWorld(cur)));
	v2f s				   = v2i(Sign(cur.x), Sign(cur.y));
	v2f f				   = v2f(fmodf(cur.x, grid.x), fmodf(cur.y, grid.y));
	v2f i				   = v2i(cur.x / grid.x, cur.y / grid.y) + s;
	v2f nearest;
	nearest.x = (i.x - (abs(f.x) < 0.5 * grid.x) * s.x) * grid.x;
	nearest.y = (i.y - (abs(f.y) < 0.5 * grid.y) * s.y) * grid.y;
	if(click) {
		TriPoint* ptptr = 0;
		for(auto& pttmp : pts) {
			if(pttmp.Get() == nearest) {
				ptptr = &pttmp;
			}
		}
		TriPoint& pt = !ptptr ? AddPoint() : *ptptr;
		if(!ptptr)
			pt.SetUp(nearest);
		TriLine* line = 0;
		TriEdge* edge = 0;
		if(last) {
			line = &AddLine();
			line->SetUp(last, &pt);
		}
		if(line) {
			edge = CAST(TriEdge*, edges.GetContent(edges.tAddLast(TriEdge())));
			edge->SetUp(line);
		}
		if(!fedge && edge) {
			fedge		= edge;
			fedge->line = line;
		};
		last = &pt;
	}
	if(rclick) {
		TriPoint* ptptr = 0;
		for(auto& pttmp : pts) {
			if(pttmp.Get() == nearest) {
				ptptr = &pttmp;
			}
		}
		TriPoint& pt = !ptptr ? AddPoint() : *ptptr;
		if(!ptptr)
			pt.SetUp(nearest);
		TriLine* line = 0;
		TriEdge* edge = 0;
		if(last) {
			line = &AddLine();
			line->SetUp(last, &pt);
			line->obj->GetComponent<Sprite>()->StopRendering();
		}
		if(line) {
			edge = CAST(TriEdge*, edges.GetContent(edges.tAddLast(TriEdge())));
			edge->SetUp(line);
		}
		if(!fedge && edge) {
			fedge		= edge;
			fedge->line = line;
		};
		last = &pt;
	}
}

void PrimManager::Clean() {
	for(auto& p : pts) {
		Scene::GetCurrent()->DeleteObject(p.idd);
	}
	for(auto& l : lines) {
		Scene::GetCurrent()->DeleteObject(l.idd);
	}
	edges.SetUp(sizeof(TriEdge), 0xFF);
	lines.clear();
	pts.clear();

	last  = 0;
	edge  = 0;
	fedge = 0;

	GameObject* obj = Scene::currentScene->GetGameObject("disc");
	if(obj)
		obj->GetComponent<Sprite>()->StopRendering();
}

void PrimManager::Process() {
	if(Inputs::GetInputOnKeyRelease('R')) {
		Clean();
	}
}

void PrimManager::Triangulate(Geo::Polygon& poly) {
	Geo::PolyOrientation		 ori = poly.CalcOrientation();
	Geo::EarClippingTriangulator ttr;
	ttr.Alloc(&poly, ori, pts.size());
	ttr.Process();
	Clean();
	printf("TriNum: %d\n", ttr.triNum);
	for(int i = 0; i < ttr.triNum * 3; i += 3) {
		TriPoint& pt0	= AddPoint();
		TriPoint& pt1	= AddPoint();
		TriPoint& pt2	= AddPoint();
		TriLine&  line0 = AddLine();
		TriLine&  line1 = AddLine();
		TriLine&  line2 = AddLine();

		pt0.SetUp(ttr.GetTris()[i]);
		pt1.SetUp(ttr.GetTris()[i + 1]);
		pt2.SetUp(ttr.GetTris()[i + 2]);
		line0.SetUp(&pt0, &pt1);
		line1.SetUp(&pt1, &pt2);
		line2.SetUp(&pt0, &pt2);
	}
	ttr.Clean();
}

void PrimManager::Update() {
	MakeLineOnClick();
	Process();
	_TestEdges();
}

TriPoint& PrimManager::AddPoint() {
	pts.push_back({});
	pts.back()._m = this;
	return pts.back();
}

TriLine& PrimManager::AddLine() {
	lines.push_back({});
	lines.back()._m = this;
	return lines.back();
}

void TriPoint::SetUp(const v2f& pos) {
	obj										  = &Scene::GetCurrent()->AddObject();
	idd										  = obj->name.c_str();
	Transform* tr							  = obj->AddComponent<Transform>();
	obj->AddComponent<Sprite>()->sortingLayer = LayerConstants::POINTS_LAYER;
	CircleRenderer* cr						  = obj->AddComponent<CircleRenderer>();
	tr->SetPosition(pos);
	cr->SetRadius(_m->ptRad);
}

v2f TriPoint::Get() { return obj->GetComponent<Transform>()->position; }

v2f* TriPoint::GetRef() { return &obj->GetComponent<Transform>()->position; }

void TriLine::SetUp() {
	obj = &Scene::GetCurrent()->AddObject();
	idd = obj->name.c_str();
	obj->AddComponent<Transform>();
	obj->AddComponent<Sprite>()->sortingLayer = LayerConstants::LINES_LAYER;
	LineRenderer* lr						  = obj->AddComponent<LineRenderer>();
	lr->SetExt(pt0->Get(), pt1->Get());
	lr->SetWidth(_m->lineWidth);
}

void TriTriangle::SetUp() {
	obj = &Scene::GetCurrent()->AddObject();
	obj->AddComponent<Transform>();
}

void TriTriangle::SetUp(TriPoint*, TriPoint*, TriPoint*) {}

void TriTriangle::SetUp(const v2f&, const v2f&, const v2f&) {}

void TriLine::SetUp(TriPoint* _pt0, TriPoint* _pt1) {
	pt0 = _pt0;
	pt1 = _pt1;
	SetUp();
}

void TriLine::SetUp(const v2f& _pt0, const v2f& _pt1) {
	pt0 = &_m->AddPoint();
	pt1 = &_m->AddPoint();
	pt0->SetUp(_pt0);
	pt1->SetUp(_pt1);
	SetUp();
}

void TriEdge::SetUp(TriLine* l) { line = l; }
