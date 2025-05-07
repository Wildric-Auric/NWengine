#include "Geometry.h"

namespace Geo {

Segment::Segment(Point* p0, Point* p1) {
    Set(p0, p1);
}

v2f* Segment::GetFstDef(Segment* s) {
    return ((Point*)s->_fpt)->Get();
}
v2f* Segment::GetSecDef(Segment* s) {
    return ((Point*)s->_spt)->Get();
}

void Segment::Set(const Point* p0, const Point* p1) {
    _fpt = (void*)p0; 
    _spt = (void*)p1; 
    GetFstProc = GetFstDef;
    GetSecProc = GetSecDef;
}

void Segment::Set(void* p0, void* p1, v2f*(*f)(Segment*), v2f*(*f2)(Segment*)) {
    _fpt = p0; 
    _spt = p1;  
    GetFstProc = f;
    GetSecProc = f2;
}

real Segment::GetSlope() const {
    return (GetFst()->y - GetSec()->y) / (GetFst()->x - GetSec()->x);
}

real Segment::GetYIntercept() const {
    return GetFst()->y - GetSlope() * GetFst()->x;
}

real Segment::Evaluate(real x) const {
    return GetSlope() * x + GetYIntercept();
}

bool Segment::IsVert() const {
   return GetFst()->x == GetSec()->x; 
}

int VertLineIntersect(const Segment& f, const Segment& s, v2r* i) {
    bool v0 = f.IsVert(); 
    bool v1 = s.IsVert();
    if (v0 && v1)
        return -1;
    if (!v0 && !v1)
        return 0;
    const Segment& vertline = v0 ? f : s;
    const Segment& line     = v0 ? s : f;

    i->x = vertline.GetFst()->x;
    i->y = line.Evaluate(i->x);

    return 1;
}

bool Segment::Intersect(const Segment& other, v2r* i) { 
    //check if lines are vertical
    int v = VertLineIntersect(*this, other, i);
    if (v == 0) {
        real m0 = GetSlope();
        real m1 = other.GetSlope();
        real b0 = GetYIntercept();
        real b1 = other.GetYIntercept();
        bool ret = 0;
        i->x = (b1 - b0) / (m0 - m1); 
        i->y = Evaluate(i->y);
    }
    return (i->x >= Max(Min(GetFst()->x,GetSec()->x),  Min(other.GetFst()->x, other.GetSec()->x)) 
         && i->x <= Min(Max(GetFst()->x, GetSec()->x), Max(other.GetFst()->x, other.GetSec()->x)));
}

v2f* Triangle::GetPtDef(void* pt) {
    return ((Point*)pt)->_v;
}

void Triangle::Set(Point** v) {
    Set((void**)v, Triangle::GetPtDef);
}

void Triangle::Set(void** v, v2f*(*f)(void*)) {
    _v[0] = v[0];
    _v[1] = v[1];
    _v[2] = v[2];
    GetPtProc = f;
}

#define STMP(p0,p1,p2) Det2((p0 - p2), (p1 - p2))
bool Triangle::IsPtInside(const v2r& pt) {
    double d0 = STMP(pt, *GetPt(0), *GetPt(1));
    double d1 = STMP(pt, *GetPt(1), *GetPt(2));
    double d2 = STMP(pt, *GetPt(2), *GetPt(0));
    bool tmp = (d0 <= 0 || d1 <= 0 || d2 <= 0) && (d0 >= 0 || d1 >= 0 || d2 >= 0);
    return !tmp;
}

}


