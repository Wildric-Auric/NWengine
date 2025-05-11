#include "Geometry.h"
#include <malloc.h>

#define nwfree(c)                                                                                                                \
	{                                                                                                                            \
		if(c != 0) {                                                                                                             \
			free((c));                                                                                                           \
			c = 0;                                                                                                               \
		}                                                                                                                        \
	}
namespace Geo {

void Point::Set(v2r* v) { _v = v; }

//-----------------Segment-----------------

Segment::Segment(Point* p0, Point* p1) { Set(p0, p1); }

v2r* Segment::GetFstDef(Segment* s) { return ((Point*)s->_fpt)->Get(); }
v2r* Segment::GetSecDef(Segment* s) { return ((Point*)s->_spt)->Get(); }

void Segment::Set(const Point* p0, const Point* p1) {
	_fpt	   = (void*)p0;
	_spt	   = (void*)p1;
	GetFstProc = GetFstDef;
	GetSecProc = GetSecDef;
}

void Segment::Set(void* p0, void* p1, v2r* (*f)(Segment*), v2r* (*f2)(Segment*)) {
	_fpt	   = p0;
	_spt	   = p1;
	GetFstProc = f;
	GetSecProc = f2;
}

real Segment::GetSlope() const { return (GetFst()->y - GetSec()->y) / (GetFst()->x - GetSec()->x); }

real Segment::GetYIntercept() const { return GetFst()->y - GetSlope() * GetFst()->x; }

real Segment::Evaluate(real x) const { return GetSlope() * x + GetYIntercept(); }

bool Segment::IsVert() const { return GetFst()->x == GetSec()->x; }

int VertLineIntersect(const Segment& f, const Segment& s, v2r* i) {
	bool v0 = f.IsVert();
	bool v1 = s.IsVert();
	if(v0 && v1)
		return -1;
	if(!v0 && !v1)
		return 0;
	const Segment& vertline = v0 ? f : s;
	const Segment& line		= v0 ? s : f;

	i->x = vertline.GetFst()->x;
	i->y = line.Evaluate(i->x);

	return 1;
}

int Segment::Intersect(const Segment& other, v2r* i) {
	// check if lines are vertical
	int v = VertLineIntersect(*this, other, i);
	if(v == 0) {
		real m0	 = GetSlope();
		real m1	 = other.GetSlope();
		real b0	 = GetYIntercept();
		real b1	 = other.GetYIntercept();
		bool ret = 0;
		i->x	 = (b1 - b0) / (m0 - m1);
		i->y	 = Evaluate(i->y);
	}

	return (i->x >= Max(Min(GetFst()->x, GetSec()->x), Min(other.GetFst()->x, other.GetSec()->x)) &&
			i->x <= Min(Max(GetFst()->x, GetSec()->x), Max(other.GetFst()->x, other.GetSec()->x))) &&
		   (i->y >= Max(Min(GetFst()->y, GetSec()->y), Min(other.GetFst()->y, other.GetSec()->y)) &&
			i->y <= Min(Max(GetFst()->y, GetSec()->y), Max(other.GetFst()->y, other.GetSec()->y)));
}

//-----------------Triangle-----------------

v2r* Triangle::GetPtDef(void* pt) { return ((Point*)pt)->_v; }

Triangle::Triangle(Point** v) { Set(v); }

void Triangle::Set(Point** v) { Set((void**)v, Triangle::GetPtDef); }

void Triangle::Set(void** v, v2r* (*f)(void*)) {
	_v[0]	  = v[0];
	_v[1]	  = v[1];
	_v[2]	  = v[2];
	GetPtProc = f;
}

#define STMP(p0, p1, p2) Det2((p0 - p2), (p1 - p2))
bool Triangle::IsPtInside(const v2r& pt) {
	double d0  = STMP(pt, *GetPt(0), *GetPt(1));
	double d1  = STMP(pt, *GetPt(1), *GetPt(2));
	double d2  = STMP(pt, *GetPt(2), *GetPt(0));
	bool   tmp = (d0 < 0 || d1 < 0 || d2 < 0) && (d0 > 0 || d1 > 0 || d2 > 0);
	return !tmp;
}
bool Triangle::IsPtInsideStrict(const v2r& pt) {
	double d0  = STMP(pt, *GetPt(0), *GetPt(1));
	double d1  = STMP(pt, *GetPt(1), *GetPt(2));
	double d2  = STMP(pt, *GetPt(2), *GetPt(0));
	bool   tmp = (d0 <= 0 || d1 <= 0 || d2 <= 0) && (d0 >= 0 || d1 >= 0 || d2 >= 0);
	return !tmp;
}
#undef STMP

//-----------------Polygon-----------------

void Polygon::SetUp(void* first, void* last, void* (*f)(Polygon*, void*), void* (*g)(Polygon*, void*),
					v2r* (*h)(Polygon*, void*)) {
	_first		= first;
	_last		= last;
	GetNextProc = f;
	GetPrevProc = g;
	UnwrapProc	= h;
}

PolyOrientation Polygon::CalcOrientation() {
	void* f	   = GetFirst();
	void* c	   = f;
	void* n	   = f;
	real  area = 0;
	do {
		n = GetNext(c);
		area += Det2(*Unwrap(c), *Unwrap(n));
		c = n;
	} while(c != f);
	if(Abs(area) < EPSILON)
		return PolyOrientation::Deg;
	if(area < 0)
		return PolyOrientation::CW;
	return PolyOrientation::CCW;
}

bool Polygon::IsPtInside(const v2r& pt) {
	void* f = GetFirst();
	void* l = GetLast();
	void* c = f;
	void* n = 0;
	v2r	  nu; // next unwrapped
	v2r	  cu;
	real  xi;	  // intersection abscissa
	int	  ic = 0; // intersection count
	do {
		n  = GetNext(c);
		cu = *Unwrap(c);
		nu = *Unwrap(n);
		if((cu.y > pt.y) == (nu.y > pt.y)) { // one pt above, the other below
			c = n;
			continue;
		}
		xi = cu.x + (pt.y - cu.y) * (nu.x - cu.x) / (nu.y - cu.y);
		ic += xi > pt.x;
		c = n;
	} while(c != f);
	return (ic % 2);
}

bool Polygon::IsConvex() {
	void* f = GetFirst();
	void* l = GetLast();

	void* c = f;
	void* n = GetNext(c);
	void* p = l;
	int	  d = Sign(Det2(*Unwrap(p) - *Unwrap(c), *Unwrap(n) - *Unwrap(c)));
	p		= c;
	c		= n;
	do {
		n = GetNext(c);
		if(Sign(Det2(*Unwrap(p) - *Unwrap(c), *Unwrap(n) - *Unwrap(c))) != d)
			return 0;
		p = c;
		c = n;
	} while(c != f);
	return 1;
}

void* Polygon::GetNextDef(Polygon* poly, void* val) {
	if(val == poly->_last) {
		return poly->_first;
	}
	return ((v2r*)(val)) + 1;
}

void* Polygon::GetPrevDef(Polygon* poly, void* val) {
	if(val == poly->_first) {
		return poly->_last;
	}
	return ((v2r*)(val)) - 1;
}

inline void Polygon::CalcData(PolygonData* d) {
	void* f			  = GetFirst();
	void* l			  = GetLast();
	void* c			  = f;
	void* n			  = GetNext(c);
	void* p			  = l;
	int	  s			  = Sign(Det2(*Unwrap(p) - *Unwrap(c), *Unwrap(n) - *Unwrap(c)));
	real  area		  = 0.0;
	p				  = c;
	c				  = n;
	d->verticesNumber = 0;
	d->centroid		  = 0.0;
	do {
		n = GetNext(c);
		area += Det2(*Unwrap(c), *Unwrap(n));
		if(Sign(Det2(*Unwrap(p) - *Unwrap(c), *Unwrap(n) - *Unwrap(c))) != s)
			d->convexity = 0;
		++d->verticesNumber;
		d->centroid = d->centroid + *Unwrap(c);
		p			= c;
		c			= n;
	} while(c != f);
	d->centroid = d->centroid / (real)d->verticesNumber;
	if(Abs(area) < EPSILON)
		d->orientation = PolyOrientation::Deg;
	else if(area < 0)
		d->orientation = PolyOrientation::CW;
	else
		d->orientation = PolyOrientation::CCW;
}

v2r* Polygon::UnwrapDef(Polygon*, void* v) { return (v2r*)(v); }

//---------------Ear Clipping------------------------

void EarClippingTriangulator::GetTri(ui32 index, v2r* p0, v2r* p1, v2r* p2) {
	*p0 = _tris[index * 3];
	*p1 = _tris[index * 3 + 1];
	*p2 = _tris[index * 3 + 2];
}

v2r EarClippingTriangulator::GetTri(ui32 index, ui32 pos) { return _tris[index * 3 + pos]; }

void EarClippingTriangulator::_SetUpCntFromPoly() {
	DirectedPoly poly;

	void* f = _poly->_first;
	void* c = f;

	_cnt[0].data = *_poly->Unwrap(c);
	_cnt[0].last = 0;
	c			 = _poly->GetNext(c);
	idx			 = 1;

	while(c != f) {
		_cnt[idx].data	   = *_poly->Unwrap(c);
		_cnt[idx].last	   = &_cnt[idx - 1];
		_cnt[idx - 1].next = &_cnt[idx];
		c				   = _poly->GetNext(c);
		++idx;
	}
	_cnt[idx - 1].next = &_cnt[0];
	_cnt[0].last	   = &_cnt[idx - 1];
}

void EarClippingTriangulator::Alloc(Geo::Polygon* const p, const PolyOrientation o, const ui32 vertn) {
	_cnt	= (DirectedPoly*)calloc(vertn, sizeof(DirectedPoly));
	_tris	= (v2r*)calloc((vertn - 2) * 3, sizeof(v2r));
	_poly	= p;
	_ort	= o;
	vertNum = vertn;
	_SetUpCntFromPoly();
}

void EarClippingTriangulator::Clean() {
	nwfree(_cnt);
	nwfree(_tris);
	idx		= 0;
	triNum	= 0;
	vertNum = 0;
}

void EarClippingTriangulator::Process(bool priorizeFans) {
	if(idx < 3)
		return;
	bool	fl;
	Point	pts0[3];
	Point*	pts[3] = {&pts0[0], &pts0[1], &pts0[2]};
	Point** ptsPtr = &pts[0];

	Triangle	  tri;
	v2r			  vec0;
	v2r			  vec1;
	i8			  s;
	DirectedPoly* c = &_cnt[0];
	DirectedPoly* tmp;
	ui32		  ti = 0;

	while(c->next->next != c->last) {
		pts0[0].Set(&c->last->data);
		pts0[1].Set(&c->data);
		pts0[2].Set(&c->next->data);
		vec0 = c->data - c->last->data;
		vec1 = c->next->data - c->data;
		s	 = Sign(Det2(vec0, vec1));
		if((_ort == PolyOrientation::CW && s == 1) || (_ort == PolyOrientation::CCW && s != 1)) {
			c = c->next;
			continue;
		}
		vec1 = (c->next->data + c->last->data) * 0.5;
		tri.Set(&pts[0]);
		tmp = c->next->next;
		fl	= 0;
		while(tmp != c->last) {
			if(!tri.IsPtInside(tmp->data)) {
				tmp = tmp->next;
				continue;
			}
			fl = 1;
			break;
		};
		if(fl) {
			c = c->next;
			continue;
		}
		_tris[ti]	  = c->last->data;
		_tris[ti + 1] = c->data;
		_tris[ti + 2] = c->next->data;
		ti += 3;
		c->last->next = c->next;
		c->next->last = c->last;
		c			  = c->next;
		if(!priorizeFans)
			c = c->next;
	}
	_tris[ti]	  = c->last->data;
	_tris[ti + 1] = c->data;
	_tris[ti + 2] = c->next->data;
	ti += 3;
	triNum = ti / 3;
}
} // namespace Geo
#undef nwfree
