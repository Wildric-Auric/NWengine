#include "Collider.h"
#include "Components.h"
#include "Maths.h"


ui32 _inf   = 0x7F800000;
ui32 _minf  = 0xFF800000;

Collider::Collider(GameObject* go) {
	this->attachedObject = go;
	Start();
};

void Collider::SetEdgesRect(const fVec2& s, const float rot) {
	fVec2 size	= fVec2(s.x * 0.5f, s.y * 0.5);
	this->edges = {fVec2(size.x, size.y).Rotate(rot), fVec2(-size.x, size.y).Rotate(rot), fVec2(-size.x, -size.y).Rotate(rot),
				   fVec2(size.x, -size.y).Rotate(rot)};
}

void Collider::SetEdgesSprite() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	if(!spr)
		return;
	Transform* tr	 = attachedObject->GetComponent<Transform>();
	fVec2	   s	 = fVec2(spr->container.width, spr->container.height);
	fVec2	   scale = fVec2(1.0f, 1.0f);
	float	   rot	 = 0;
	if(tr) {
		scale = tr->scale;
		rot	  = tr->rotation;
	}
	SetEdgesRect(scale * s, rot);
}

void Collider::Start() {
	Transform* transform = this->attachedObject->AddComponent<Transform>();
	SetEdgesSprite();
}

fVec2 Collider::GetPosition() {
	Transform* trans = this->attachedObject->GetComponent<Transform>();
	return trans->position + offset;
}

fVec2 Collider::GetEdgePosition(int index) { return GetPosition() + this->edges[index]; }

bool Collider::Sat(Collider* other, fVec2* depthBuffer) {
	float depth = *(float*)&_inf;
	for(int i = 0; i < this->edges.size() - 1; ++i) {
		fVec2 side	 = GetEdgePosition((i + 1) % edges.size()) - GetEdgePosition(i);
		fVec2 normal = fVec2(side.y, -side.x).normalize();
		float minn	 = *(float*)&_inf;
		float maxx	 = *(float*)&_minf;
		float min0	 = *(float*)&_inf;
		float max0	 = *(float*)&_minf;

		for(int j = 0; j < this->edges.size(); ++j) { // TODO::Improve this loop
			fVec2 vec  = GetEdgePosition(j) - GetEdgePosition(i);
			float proj = vec.Dot(normal);

			if(proj <= minn)
				minn = proj;

			if(proj >= maxx)
				maxx = proj;
		}

		for(int j = 0; j < other->edges.size(); ++j) {

			fVec2 vec  = other->GetEdgePosition(j) - GetEdgePosition(i);
			float proj = vec.Dot(normal);

			if(proj <= min0)
				min0 = proj;

			if(proj >= max0)
				max0 = proj;
		}
		float temp = Min<float>(maxx - min0, max0 - minn);
		if(temp <= depth) {
			depth		 = temp;
			*depthBuffer = normal * depth;
		}
		if(temp <= 0.0) {
			return false;
		}
	}
	return true;
}

bool Collider::isColliding(Collider* other, fVec2* depthBuffer) {
	fVec2 b0;
	fVec2 b1;
	fVec2 temp;

	if(depthBuffer == nullptr)
		depthBuffer = &temp;
	fVec2 dir = (this->GetPosition() - other->GetPosition()).normalize();

	if(!other->Sat(this, &b1) || !Sat(other, &b0))
		return false;

	if(b0.magnitude() < b1.magnitude())
		*depthBuffer = b0 * Sign<float>(dir.Dot(b0));
	else
		*depthBuffer = b1 * Sign<float>(dir.Dot(b1));
	return true;
}

void Collider::AddVertex(const fVec2& v) { edges.push_back(v); }

bool Collider::isInside(const fVec2& point) {
	fVec2* cur;
	fVec2* nex;
	fVec2  dir;
	fVec2  dir0;
	for(int i = 0; i < edges.size() - 1; ++i) {
		cur	 = &edges[i];
		nex	 = &edges[i + 1];
		dir	 = (*nex - *cur).normalize();
		dir0 = (point - *cur).normalize();
		if(Det2(dir0, dir) > 0.0)
			return 0;
	}
	return 1;
}

float CircleCollider::Sat(const fVec2& axis, const fVec2& origin, Collider* other) {
	float minn = *(float*)&_inf;
	float maxx = *(float*)&_minf;
	float min0 = *(float*)&_inf;
	float max0 = *(float*)&_minf;
	// Project polygon and find min max
	for(int j = 0; j < other->edges.size(); ++j) {
		fVec2 vec  = other->GetEdgePosition(j) - origin;
		float proj = vec.Dot(axis);
		if(proj <= minn)
			minn = proj;
		if(proj >= maxx)
			maxx = proj;
	}
	// Project circle and find min and max
	float projCenter = (GetPosition() - origin).Dot(axis);
	float proj0		 = projCenter + radius; // projCenter + (radius * axis).Dot(axis);
	float proj1		 = projCenter - radius;
	min0			 = (proj0 <= min0) ? proj0 : min0;
	max0			 = (proj0 >= max0) ? proj0 : max0;
	min0			 = (proj1 <= min0) ? proj1 : min0;
	max0			 = (proj1 >= max0) ? proj1 : max0;
	return Min<float>(maxx - min0, max0 - minn);
}

bool CircleCollider::isColliding(Collider* other, fVec2* depthBuffer) {

	float depth = *(float*)&_inf;
	fVec2 tmp;
	int	  nearestEdge = -1;
	float nearestDist = *(float*)&_inf;

	if(depthBuffer == 0)
		depthBuffer = &tmp;
	// Polygon axis
	for(int i = 0; i < other->edges.size(); ++i) {
		fVec2 axis	 = other->GetEdgePosition((i + 1) % other->edges.size()) - other->GetEdgePosition(i);
		fVec2 normal = fVec2(axis.y, -axis.x).normalize();
		float temp	 = Sat(normal, other->GetEdgePosition(i), other);
		if(temp <= depth) {
			depth		 = temp;
			*depthBuffer = normal * depth;
		}
		if(temp <= 0.0) {
			*depthBuffer = fVec2(0.0f, 0.0f);
			return false;
		}
		float d = (GetPosition() - other->GetEdgePosition(i)).magnitude();
		if(d > nearestDist)
			continue;
		nearestDist = d;
		nearestEdge = i;
	}
	// Circle axis
	fVec2 axis = (other->GetEdgePosition(nearestEdge) - GetPosition()).normalize();
	// fVec2 normal = fVec2(axis.y, -axis.x).normalize();
	float temp = Sat(axis, GetPosition(), other);
	if(temp <= 0.0) {
		*depthBuffer = fVec2(0.0f, 0.0f);
		return false;
	}
	if(temp <= depth) {
		depth		 = temp;
		*depthBuffer = axis * depth;
	}
	*depthBuffer = *depthBuffer * -Sign<float>(axis.Dot(*depthBuffer));

	return true;
}

bool CircleCollider::isInside(const fVec2& p) { return (p - GetPosition()).magnitude() < radius; }

void CircleCollider::Start() {}

fVec2 CircleCollider::GetPosition() { return attachedObject->GetComponent<Transform>()->position; }

void CircleCollider::SetRadius(float val) { radius = val; }

bool CircleCollider::isColliding(CircleCollider* other, fVec2* depthBuffer) {
	fVec2 a	   = GetPosition();
	fVec2 b	   = other->GetPosition();
	fVec2 diff = (GetPosition() - other->GetPosition());
	float m	   = diff.magnitude();
	bool  v	   = m <= (radius + other->radius);
	if(!v)
		return 0;
	*depthBuffer = diff.normalize() * (radius + other->radius - m);
	return 1;
}

bool ConcaveCollider::isColliding(Collider* other, v2f* depth) {
	bool b = 0;
	for(Collider& col : cvxCols) {
		b = col.isColliding(other, depth);
		if(b)
			return b;
	}
	return b;
};

bool ConcaveCollider::isColliding(CircleCollider* other, v2f* depth) {
	bool b = 0;
	for(Collider& col : cvxCols) {
		b = other->isColliding(&col, depth);
		if(b)
			return b;
	}
	return b;
};

bool ConcaveCollider::isColliding(ConcaveCollider* cols, v2f* depth) {
	bool b = 0;
	for(Collider& col : cvxCols) {
		for(Collider& other : cols->cvxCols) {
			b = col.isColliding(&other, depth);
			if(b)
				return b;
		}
	}
	return b;
};

bool ConcaveCollider::isInside(const fVec2& pt) {
	for(Collider& col : cvxCols) {
		if(col.isInside(pt))
			return 1;
	}
	return 0;
};

void ConcaveCollider::AddVertex(const fVec2& v) { vertices.push_back(v); }

void ConcaveCollider::Partition() {
	Geo::Polygon poly;
	poly.SetFirst(vertices.data());
	poly.SetLast(&vertices.back());
	Partition(poly);
}

void ConcaveCollider::Partition(Geo::Polygon& poly) {
	Geo::PolyOrientation ori = poly.CalcOrientation();
	Partition(poly, ori);
}

void ConcaveCollider::Partition(Geo::Polygon& poly, const Geo::PolyOrientation ori) {
	Geo::EarClippingTriangulator ttr;
	Collider*					 col;
	cvxCols.clear();
	ttr.Alloc(&poly, ori, vertices.size());
	ttr.Process();
	for(int i = 0; i < ttr.triNum * 3; i += 3) {
		cvxCols.emplace_back();
		col = &cvxCols.back();
		col->SetGameObject(attachedObject);
		col->edges.emplace_back(ttr.GetTri(i, 0));
		col->edges.emplace_back(ttr.GetTri(i, 1));
		col->edges.emplace_back(ttr.GetTri(i, 2));
	}
	ttr.Clean();
}
