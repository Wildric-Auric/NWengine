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
static TriEdge* edge;
static TriEdge* fedge; 

void PrimManager::_TestEdges() {
    if (Inputs::GetInputKey('C', NWin::KeyEventEnum::NWIN_KeyPressed)) {
        addr e = edges.GetFirst();
        int sss = 0;
        while (e) {
            if (sss == int(tmpf) - 1) {
                TriEdge* ppp = CAST(TriEdge*, edges.GetContent(e));
                ppp->line->obj->GetComponent<LineRenderer>()->SetWidth(ppp->line->_m->lineWidth);
            }
            if (sss == int(tmpf)) {
                TriEdge* ppp = CAST(TriEdge*, edges.GetContent(e));
                ppp->line->obj->GetComponent<LineRenderer>()->SetWidth(0);
            }
            e = edges.GetNext(e);
            sss++;
        }
        tmpf += NWTime::GetDeltaTime();
        if (tmpf >= edges._size) {
            tmpf = 0.0;
            TriEdge* ppp = CAST(TriEdge*, edges.GetContent(edges.GetLast()));
            ppp->line->obj->GetComponent<LineRenderer>()->SetWidth(ppp->line->_m->lineWidth);
        }
    }
}

void PrimManager::MakeLineOnClick() {
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
       TriLine* line = 0;
       TriEdge* edge = 0;
       if (last) {
           line = &AddLine();
           line->SetUp(last, &pt);
       }
       if (line) {
            edge = CAST(TriEdge*,edges.GetContent(edges.tAddLast(TriEdge())));
            edge->SetUp(line);
       }
       if (!fedge && edge) {fedge = edge; fedge->line = line;};
       last = &pt;
    }
}

void PrimManager::Update() {
    MakeLineOnClick();
    _TestEdges();
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

void TriTriangle::SetUp() {
    obj = &Scene::GetCurrent()->AddObject();
    obj->AddComponent<Transform>();
}

void TriTriangle::SetUp(TriPoint*, TriPoint*, TriPoint*) {

}

void TriTriangle::SetUp(const v2f&, const v2f&, const v2f&) {

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

void TriEdge::SetUp(TriLine* l) {
    line = l;
}
