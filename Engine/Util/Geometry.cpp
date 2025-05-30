#include "Geometry.h"
#include <malloc.h>
#include <vector>
#include <map>
#include <list>

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
		i->y	 = Evaluate(i->x);
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

bool Triangle::IsDegenerate() {
	v2r a = *GetPt(0);
	v2r b = *GetPt(1);
	v2r c = *GetPt(2);
	return ABS(Det2(b - a, b - c)) < GEO_EPS;
}

v2r Triangle::CalcCircCenter() {
	v2r a	   = *GetPt(0);
	v2r b	   = *GetPt(1);
	v2r c	   = *GetPt(2);
	v2r va	   = (a - b);
	v2r vb	   = (a - c);
	v2r mpa	   = (a + b) * 0.5;
	v2r mpb	   = (a + c) * 0.5;
	va		   = v2r(-va.y, va.x);
	vb		   = v2r(-vb.y, vb.x);
	v2r	  mpas = mpa + va;
	v2r	  mpbs = mpb + vb;
	Point mpapt0;
	mpapt0.Set(&mpa);
	Point mpapt1;
	mpapt1.Set(&mpas);
	Segment la(&mpapt0, &mpapt1);
	Point	mpbpt0;
	mpbpt0.Set(&mpb);
	Point mpbpt1;
	mpbpt1.Set(&mpbs);
	Segment lb(&mpbpt0, &mpbpt1);
	v2r		insct;
	la.Intersect(lb, &insct);
	return insct;
}
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
		tri.Set(&pts[0]);
		if(tri.IsDegenerate()) {
			c = c->next;
			continue;
		}
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

//---------------Delaunay---------------

void DelaunayTriangulator::GetTri(ui32 index, v2r* p0, v2r* p1, v2r* p2) {
	*p0 = _tris[index * 3 + 0];
	*p1 = _tris[index * 3 + 1];
	*p2 = _tris[index * 3 + 2];
}
void DelaunayTriangulator::Alloc(PointSet* const s, const ui32 num) {
	ptsNum	 = num;
	_ptSet	 = s;
	_trisCap = (2 * num + 1) * 3;
	_tris	 = (v2r*)calloc(_trisCap, sizeof(v2r));
}

#define CHK(pt) (pt) == tri.pts[0] || (pt) == tri.pts[1] || (pt) == tri.pts[2]
void DelaunayTriangulator::DeleteSuper(const TriangleData& tri) {
	int	 n	   = triNum;
	int	 c	   = 0;
	int	 indx  = 0;
	v2r* ntris = (v2r*)calloc(triNum * 3, sizeof(v2r));
	for(int i = 0; i < n; ++i) {
		if(CHK(_tris[i * 3 + 0]) || (CHK(_tris[i * 3 + 1])) || (CHK(_tris[i * 3 + 2]))) {
			continue;
		}
		ntris[indx++] = _tris[i * 3];
		ntris[indx++] = _tris[i * 3 + 1];
		ntris[indx++] = _tris[i * 3 + 2];
		c++;
	}
	nwfree(_tris);
	_tris  = ntris;
	triNum = c;
}
#undef CHK

void DelaunayTriangulator::Clean() { nwfree(_tris); }
void DelaunayTriangulator::Process() {
	TriangleData trid;
	ComputeSuperTriangle(&trid);
	Process(trid);
	DeleteSuper(trid);
}

#define REALC                                                                                                                    \
	if(_trisCap < triNum * 3 + 3) {                                                                                              \
		_trisCap = _trisCap * 2 + 3;                                                                                             \
		_tris	 = (v2r*)realloc(_tris, _trisCap * sizeof(v2r));                                                                 \
	}

void DelaunayTriangulator::_AddTri(Triangle& tri) {
	REALC;
	_tris[triNum * 3 + 0] = *tri.GetPt(0);
	_tris[triNum * 3 + 1] = *tri.GetPt(1);
	_tris[triNum * 3 + 2] = *tri.GetPt(2);
	triNum				  = triNum + 1;
}

void DelaunayTriangulator::_AddTri(TriangleData& tri) {
	REALC;
	_tris[triNum * 3 + 0] = tri.pts[0];
	_tris[triNum * 3 + 1] = tri.pts[1];
	_tris[triNum * 3 + 2] = tri.pts[2];
	triNum				  = triNum + 1;
}
#undef REALC

struct EdgeComp {
	bool operator()(std::pair<v2r, v2r> const& p1, std::pair<v2r, v2r> const& p2) const {
		// compare p1.first agnst p2.first, then p1.second against p2.second
		if(p1.first.x < p2.first.x)
			return true;
		if(p1.first.x > p2.first.x)
			return false;
		if(p1.first.y < p2.first.y)
			return true;
		if(p1.first.y > p2.first.y)
			return false;
		return p1.second.x < p2.second.x || (p1.second.x == p2.second.x && p1.second.y < p2.second.y);
	}
};

static std::pair<v2r, v2r> MakeKey(const v2r& p, const v2r& q) {
	return (p.x < q.x || (p.x == q.x && p.y < q.y)) ? std::make_pair(p, q) : std::make_pair(q, p);
}

void ProcessBadTris(std::vector<v2r>* tris) {
	auto&										 pts = *tris;
	std::map<std::pair<v2r, v2r>, int, EdgeComp> edgeCount;
	for(size_t i = 0; i + 2 < pts.size(); i += 3) {
		edgeCount[MakeKey(pts[i], pts[i + 1])]++;
		edgeCount[MakeKey(pts[i + 1], pts[i + 2])]++;
		edgeCount[MakeKey(pts[i + 2], pts[i])]++;
	}
	tris->clear();
	tris->reserve(edgeCount.size() * 100);
	for(const auto& kv : edgeCount) {
		if(kv.second != 1)
			continue;
		tris->push_back(kv.first.first);
		tris->push_back(kv.first.second);
	}
}

void DelaunayTriangulator::Process(TriangleData& superTri) {
	void*					f = _ptSet->GetFirst();
	void*					l = _ptSet->GetLast();
	void*					c = f;
	void*					n;
	v2r						uc;
	TriangleData			tri = superTri;
	std::list<TriangleData> tris;
	std::vector<v2r>		badtris;
	tris.push_back(superTri);
	Point	 pt[3];
	Point*	 ptr[3];
	Triangle tr;
	v2r		 center;
	real	 dist;
	real	 rad;
	for(int i = 0; i < ptsNum; ++i) {
		n  = _ptSet->GetNext(c);
		uc = *_ptSet->Unwrap(c);
		badtris.clear();
		for(auto tri = tris.begin(); tri != tris.end();) {
			pt[0].Set(&tri->pts[0]);
			pt[1].Set(&tri->pts[1]);
			pt[2].Set(&tri->pts[2]);
			ptr[0] = &pt[0];
			ptr[1] = &pt[1];
			ptr[2] = &pt[2];
			tr.Set(ptr);
			center = tr.CalcCircCenter();
			dist   = (center - uc).magnitude();
			rad	   = (center - *tr.GetPt(0)).magnitude();
			if(dist >= rad) {
				++tri;
				continue;
			}
			badtris.push_back(*pt[0].Get());
			badtris.push_back(*pt[1].Get());
			badtris.push_back(*pt[2].Get());
			tri = tris.erase(tri);
		}
		if(badtris.size() == 0) {
			c = n;
			continue;
		}
		ProcessBadTris(&badtris);
		for(size_t i = 0; i + 1 < badtris.size(); i += 2) {
			tris.push_back({{uc, badtris[i], badtris[i + 1]}});
		}
		c = n;
	}
	for(TriangleData& d : tris) {
		_AddTri(d);
	}
	// triNum = tris.size();
}

void DelaunayTriangulator::ComputeSuperTriangle(TriangleData* tri) {
	void* f = _ptSet->GetFirst();
	void* c = f;
	void* n;

	v2r v;	// unwrapped value
	v2r rx; // range of x
	v2r ry; // range of y
	do {
		n	 = _ptSet->GetNext(c);
		v	 = *_ptSet->Unwrap(c);
		rx.x = MMIN(rx.x, v.x);
		rx.y = MMAX(rx.y, v.x);
		ry.x = MMIN(ry.x, v.y);
		ry.y = MMAX(ry.y, v.y);
		c	 = n;
	} while(c != f);
	tri->pts[0].x = (rx.x + rx.y) * 0.5;
	tri->pts[0].y = (-ry.x + ry.y) + ry.y;
	tri->pts[1].x = rx.y + (rx.y - rx.x) * 0.5;
	tri->pts[1].y = ry.x;
	tri->pts[2].x = rx.x - (rx.y - rx.x) * 0.5;
	tri->pts[2].y = ry.x;

	tri->pts[2].x -= supOffset;
	tri->pts[2].y -= supOffset;
	tri->pts[1].x += supOffset;
	tri->pts[1].y -= supOffset;
}

} // namespace Geo
#undef nwfree
