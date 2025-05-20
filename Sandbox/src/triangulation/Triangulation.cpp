#include "Triangulation.h"
    
namespace Geo {
void ConstEarClippingTriangulator::Process(bool(*cond)(Point*,Point*)) {
	if(tlgr.idx < 3)
		return;
	bool	fl;
	Point	pts0[3];
	Point*	pts[3] = {&pts0[0], &pts0[1], &pts0[2]};
	Point** ptsPtr = &pts[0];

	Triangle	  tri;
	v2r			  vec0;
	v2r			  vec1;
	i8			  s;
	DirectedPoly* c = &tlgr._cnt[0];
	DirectedPoly* tmp;
	ui32		  ti = 0;

    int i = 0;
	while(c->next->next != c->last && i < 1000 ) {
        i++;
		pts0[0].Set(&c->last->data);
		pts0[1].Set(&c->data);
		pts0[2].Set(&c->next->data);
		vec0 = c->data - c->last->data;
		vec1 = c->next->data - c->data;
		s	 = Sign(Det2(vec0, vec1));
		if((tlgr._ort == PolyOrientation::CW && s == 1) || (tlgr._ort == PolyOrientation::CCW && s != 1)) {
			c = c->next;
			continue;
		}
        if (cond(&pts0[0],&pts0[1])) {
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
		tlgr._tris[ti]	  = c->last->data;
		tlgr._tris[ti + 1] = c->data;
		tlgr._tris[ti + 2] = c->next->data;
		ti += 3;
		c->last->next = c->next;
		c->next->last = c->last;
		c			  = c->next;
//		if(!priorizeFans)
//			c = c->next;
	}
	tlgr._tris[ti]	  = c->last->data;
	tlgr._tris[ti + 1] = c->data;
	tlgr._tris[ti + 2] = c->next->data;
	ti += 3;
	tlgr.triNum = ti / 3;
};
}
