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
	bool IsPtInside(const v2r& pt);
};
} // namespace Geo
