#pragma once
#include "Globals.h"
#include "ds.h"

namespace Geo {
extern MemoryRegion heap;

class Point {
  public:
	v2r*		_v = 0;
	inline v2r* Get() { return _v; }
	inline v2r	GetUnwrp() { return *_v; }
	void		Set(v2f*);
};

class Segment {
  public:
	void* _fpt = 0;
	void* _spt = 0;

	v2f* (*GetFstProc)(Segment*) = GetFstDef;
	v2f* (*GetSecProc)(Segment*) = GetSecDef;
	static v2f* GetFstDef(Segment*);
	static v2f* GetSecDef(Segment*);

	Segment(Point*, Point*);
	Segment() = default;
	inline v2f* GetFst() const { return GetFstProc((Segment*)this); }
	inline v2f* GetSec() const { return GetSecProc((Segment*)this); }
	void		Set(const Point*, const Point*);
	void		Set(void* p0, void* p1, v2f* (*f)(Segment*), v2f* (*f2)(Segment*));
	real		GetSlope() const;
	real		GetYIntercept() const; // ordinate at origin
	real		Evaluate(real x) const;
	bool		IsVert() const;
	int			Intersect(const Segment& other, v2r* i);
};

class Triangle {
  public:
	void* _v[3];

	v2f* (*GetPtProc)(void*) = GetPtDef;
	static v2f* GetPtDef(void*);

	Triangle(Point**);
	Triangle() = default;

	inline v2f* GetPt(const int index = 0) { return GetPtDef(_v[index]); }

	void Set(Point**);
	void Set(void**, v2f* (*)(void*));
	bool IsPtInside(const v2r&);
	bool IsPtInsideStrict(const v2r&);
};

enum PolyOrientation {
	CCW = -1,
	CW	= 1,
	Deg = 0 // Degenerate
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
	v2f			  data;
	DirectedPoly* next = 0;
	DirectedPoly* last = 0;
};

class EarClippingTriangulator {
  public:
	DirectedPoly*	_cnt  = 0;
	v2f*			_tris = 0;
	Polygon*		_poly = 0;
	PolyOrientation _ort;
	ui32			idx		= 0;
	ui32			triNum	= 0;
	ui32			vertNum = 0;

	inline ui32 GetTriNum() { return triNum; }
	inline v2f* GetTris() { return _tris; }
	void		Alloc(Polygon* const, const PolyOrientation, const ui32 vertn);
	void		Clean();
	void		Process(bool priorizeFans = 0);
	void		_SetUpCntFromPoly();
};

} // namespace Geo
