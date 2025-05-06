#pragma "once"
#include "Script.h"
#include "Globals.h"
#include <list>
#include "ds.h"

enum LayerConstants {
    POINTS_LAYER= 0xFFFF,
    LINES_LAYER = 0xFF00,
    BLUEPRINT   = NW_I32_MAX,
};

class PrimManager;

class TriPoint {
    public:
    PrimManager* _m;
    GameObject* obj;
    void SetUp();
    void SetUp(const v2f&);
    v2f Get();
};

class TriLine {
    public:
    GameObject* obj;
    PrimManager* _m;
    TriPoint* pt0;
    TriPoint* pt1;
    void SetUp(TriPoint*, TriPoint*);
    void SetUp(const v2f&, const v2f&);
    void SetUp();
};

class TriTriangle {
    public:
    GameObject*  obj;
    PrimManager* _m;
    TriPoint* pt0;
    TriPoint* pt1;
    TriPoint* pt3;

    void SetUp();
    void SetUp(TriPoint*, TriPoint*, TriPoint*);
    void SetUp(const v2f&, const v2f&, const v2f&);
};

class TriEdge {
    public:
    TriLine* line = 0;
    void SetUp(TriLine* l);
};

class PrimManager: public Scriptable {
public:
    SCRIPT_CONSTR(PrimManager)
    float lineWidth = 4.0f;
    float ptRad     = 8.0f;
    v2f   grid      = v2f(50.0f, 50.0f);
    std::list<TriPoint> pts;
    std::list<TriLine> lines;
    DList edges;

    TriPoint& AddPoint();
    TriLine&  AddLine();
    void Start()  override;
    void Update() override;
    void MakeLineOnClick();

    //debug methods
    void _TestEdges();
private:
    float tmpf = 0;
};
