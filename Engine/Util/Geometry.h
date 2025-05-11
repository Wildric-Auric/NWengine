#pragma once
#include "Globals.h"

#define GEO_EPS 0.00001

namespace Geo {

class Point {
  public:
	v2r*		_v = 0;
	inline v2r* Get() { return _v; }
	inline v2r	GetUnwrp() { return *_v; }
	void		Set(v2r*);
};

class Segment {
  public:
	void* _fpt = 0;
	void* _spt = 0;

	v2r* (*GetFstProc)(Segment*) = GetFstDef;
	v2r* (*GetSecProc)(Segment*) = GetSecDef;
	static v2r* GetFstDef(Segment*);
	static v2r* GetSecDef(Segment*);

	Segment(Point*, Point*);
	Segment() = default;
	inline v2r* GetFst() const { return GetFstProc((Segment*)this); }
	inline v2r* GetSec() const { return GetSecProc((Segment*)this); }
	void		Set(const Point*, const Point*);
	void		Set(void* p0, void* p1, v2r* (*f)(Segment*), v2r* (*f2)(Segment*));
	real		GetSlope() const;
	real		GetYIntercept() const; // ordinate at origin
	real		Evaluate(real x) const;
	bool		IsVert() const;
	int			Intersect(const Segment& other, v2r* i);
};

class Triangle {
  public:
	void* _v[3];

	v2r* (*GetPtProc)(void*) = GetPtDef;
	static v2r* GetPtDef(void*);

	Triangle(Point**);
	Triangle() = default;

	inline v2r* GetPt(const int index = 0) { return GetPtProc(_v[index]); }

	void Set(Point**);
	void Set(void**, v2r* (*)(void*));
	bool IsPtInside(const v2r&);
	bool IsPtInsideStrict(const v2r&);
	bool IsDegenerate();
};

enum PolyOrientation {
	CCW = -1,
	CW	= 1,
	Deg = 0 // Degenerate
};

struct PolygonData {
	PolyOrientation orientation;
	ui32			verticesNumber;
	bool			convexity;
	v2r				centroid;
};

class Polygon {
  public:
	void* _first;
	void* _last;
	void* _data;

	void* (*GetNextProc)(Polygon*, void*) = GetNextDef;
	void* (*GetPrevProc)(Polygon*, void*) = GetPrevDef;
	v2r* (*UnwrapProc)(Polygon*, void*)	  = UnwrapDef;

	inline v2r* GetFirstUnwrp() { return Unwrap(_first); }
	inline v2r* GetLastUnwrp() { return Unwrap(_last); }
	inline v2r* GetNextUnwrp(void* n) { return Unwrap(GetNext(n)); }
	inline v2r* GetPrevUnwrp(void* n) { return Unwrap(GetPrev(n)); }

	inline void* GetFirst() { return _first; }
	inline void* GetLast() { return _last; }
	inline void* GetNext(void* v) { return GetNextProc(this, v); }
	inline void* GetPrev(void* v) { return GetPrevProc(this, v); }
	inline v2r*	 Unwrap(void* v) { return UnwrapProc(this, v); }
	inline void	 SetFirst(void* f) { _first = f; }
	inline void	 SetLast(void* l) { _last = l; }
	inline void	 SetData(void* d) { _data = d; }
	inline void	 SetGetNextProc(void* (*f)(Polygon*, void*)) { GetNextProc = f; }
	inline void	 SetGetPrevProc(void* (*f)(Polygon*, void*)) { GetPrevProc = f; }
	inline void	 SetUnwrapProc(v2r* (*f)(Polygon*, void*)) { UnwrapProc = f; }
	inline void	 CalcData(PolygonData*);

	void			SetUp(void* first, void* last, void* (*getNext)(Polygon*, void*), void* (*)(Polygon* getPrev, void*),
						  v2r* (*)(Polygon* getUnwrp, void*));
	PolyOrientation CalcOrientation();
	bool			IsPtInside(const v2r& pt);
	bool			IsConvex();
	static void*	GetNextDef(Polygon*, void*);
	static void*	GetPrevDef(Polygon*, void*);
	static v2r*		UnwrapDef(Polygon*, void*);
};

struct DirectedPoly {
	v2r			  data;
	DirectedPoly* next = 0;
	DirectedPoly* last = 0;
};

class EarClippingTriangulator {
  public:
	DirectedPoly*	_cnt  = 0;
	v2r*			_tris = 0;
	Polygon*		_poly = 0;
	PolyOrientation _ort;
	ui32			idx		= 0;
	ui32			triNum	= 0;
	ui32			vertNum = 0;

	inline ui32 GetTriNum() { return triNum; }
	inline v2r* GetTris() { return _tris; }
	void		GetTri(ui32 index, v2r* p0, v2r* p1, v2r* p2);
	v2r			GetTri(ui32 index, ui32 pos);
	void		Alloc(Polygon* const, const PolyOrientation, const ui32 vertn);
	void		Clean();
	void		Process(bool priorizeFans = 0);
	void		_SetUpCntFromPoly();
};

} // namespace Geo
