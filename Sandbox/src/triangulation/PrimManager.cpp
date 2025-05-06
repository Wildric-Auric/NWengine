#include "PrimManager.h"
#include "Scene.h"
#include "Transform.h"
#include "LineRenderer.h"
#include "CircleRenderer.h"
#include "Inputs.h"
#include "CoordSys.h"
#include "Maths.h"
#include "keyboard.h"

void PrimManager::Start() {
//    TriPoint& pt = AddPoint();
//    TriPoint& pt1 = AddPoint();
//    pt.SetUp(v2f(0.0,0.0));
//    pt1.SetUp(v2f(100.0,0.0));
//    TriLine& l = AddLine();
//    l.SetUp(&pt, &pt1); 
}

static TriPoint* last;
void PrimManager::Update() {
    constexpr float tol = 0.5;
    bool click = Inputs::GetInputKey(NWin::NWIN_KEY_LBUTTON, NWin::KeyEventEnum::NWIN_KeyReleased);
    fVec2 cur = Inputs::GetMousePosition();
	cur = NWCoordSys::WorldToViewportNonNormalized((NWCoordSys::ScreenNonNormalizedToWorld(cur)));
    v2f s = v2i(Sign(cur.x), Sign(cur.y));
    v2f f = v2f(fmodf(cur.x, grid.x), fmodf(cur.y, grid.y)); 
    v2f i = v2i(cur.x / grid.x, cur.y / grid.y) + s;
    v2f nearest;
    nearest.x = (i.x - (abs(f.x) < 0.5 * grid.x) * s.x) * grid.x;
    nearest.y = (i.y - (abs(f.y) < 0.5 * grid.y) * s.y) * grid.y; 
    if (click) {
       TriPoint& pt = AddPoint();
       pt.SetUp(nearest);
       if (last) {
           AddLine().SetUp(last, &pt);
       }
       last = &pt;
    }
}

TriPoint& PrimManager::AddPoint() {
    pts.push_back({});
    pts.back()._m = this;
    return pts.back();
}

TriLine&  PrimManager::AddLine() {
    lines.push_back({});
    lines.back()._m = this;
    return lines.back();
}

void TriPoint::SetUp(const v2f& pos) {
    obj = &Scene::GetCurrent()->AddObject();
    Transform* tr = obj->AddComponent<Transform>();
    obj->AddComponent<Sprite>()->sortingLayer = LayerConstants::POINTS_LAYER;
    CircleRenderer* cr = obj->AddComponent<CircleRenderer>();
    tr->SetPosition(pos);
    cr->SetRadius(_m->ptRad);
}

v2f TriPoint::Get() {
    return obj->GetComponent<Transform>()->position;
}

void TriLine::SetUp() {
    obj = &Scene::GetCurrent()->AddObject();
    obj->AddComponent<Transform>();
    obj->AddComponent<Sprite>()->sortingLayer = LayerConstants::LINES_LAYER;
    LineRenderer* lr = obj->AddComponent<LineRenderer>();
    lr->SetExt(pt0->Get(), pt1->Get());
    lr->SetWidth(_m->lineWidth); 
}

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
