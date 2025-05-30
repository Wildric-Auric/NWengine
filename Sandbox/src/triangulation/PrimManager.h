#pragma once
#include "Script.h"
#include "Globals.h"
#include "Geometry.h"
#include "ds.h"
#include <list>

enum LayerConstants {
	POINTS_LAYER = 0xFFFF,
	LINES_LAYER	 = 0xFF00,
	BLUEPRINT	 = NW_I32_MAX,
};

class PrimManager;

class TriPoint {
  public:
	PrimManager* _m;
	GameObject*	 obj;
	const char*	 idd;
	void		 SetUp();
	void		 SetUp(const v2f&);
	v2f			 Get();
	v2f*		 GetRef();
};

class TriLine {
  public:
	GameObject*	 obj;
	PrimManager* _m;
	TriPoint*	 pt0;
	TriPoint*	 pt1;
	const char*	 idd;
	void		 SetUp(TriPoint*, TriPoint*);
	void		 SetUp(const v2f&, const v2f&);
	void		 SetUp();
};

class TriTriangle {
  public:
	GameObject*	 obj;
	PrimManager* _m;
	TriPoint*	 pt0;
	TriPoint*	 pt1;
	TriPoint*	 pt3;

	const char* idd;

	void SetUp();
	void SetUp(TriPoint*, TriPoint*, TriPoint*);
	void SetUp(const v2f&, const v2f&, const v2f&);
};

class TriEdge {
  public:
	TriLine* line = 0;
	void	 SetUp(TriLine* l);
};

class PrimManager : public Scriptable {
  public:
	SCRIPT_CONSTR(PrimManager)
	float				lineWidth = 1.0f;
	float				ptRad	  = 5.0f;
	v2f					grid	  = v2f(50.0f, 50.0f);
	v4f					lineCol   = v4f(1.0,1.0,1.0,1.0);
	v4f					ptCol     = v4f(1.0,1.0,1.0,10);
	std::list<TriPoint> pts;
	std::list<TriLine>	lines;
    int nbLine = 0;
	DList				edges;

	TriPoint& AddPoint();
	TriLine&  AddLine();
	void	  Start() override;
	void	  Update() override;
	void	  MakeLineOnClick();
	void	  Process();
	TriPoint* FindPt(const v2r&);
	void	  Triangulate(Geo::Polygon&);
	void	  Clean();

	// debug methods
	void _TestEdges();

  private:
	float tmpf = 0;
};
