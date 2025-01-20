#include "Collider.h"
#include "Maths.h"
#include "Components.h"

Collider::Collider(GameObject* go) {
	this->attachedObject = go;
	Start();
};


void Collider::SetEdgesRect(const fVec2& s, const float rot) {
    fVec2 size = fVec2(s.x * 0.5f, s.y * 0.5);
	this->edges = {
		fVec2(size.x , size.y).Rotate(rot) ,
		fVec2(-size.x, size.y).Rotate(rot) ,
		fVec2(-size.x, -size.y).Rotate(rot),
		fVec2(size.x , -size.y).Rotate(rot)
	};
}

void Collider::SetEdgesSprite() {
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    if (!spr) return;
    Transform* tr = attachedObject->GetComponent<Transform>();
    fVec2 s = fVec2(spr->container.width, spr->container.height);
    fVec2 scale = fVec2(1.0f,1.0f);
    float rot = 0;
    if (tr) {
        scale = tr->scale;
        rot   = tr->rotation;
    }
    SetEdgesRect(scale * s, rot);
}

void Collider::Start() {
	Transform* transform = this->attachedObject->GetComponent<Transform>();
    SetEdgesSprite();
}

fVec2 Collider::GetPosition() {
	Transform* trans = this->attachedObject->GetComponent<Transform>();
	return trans->position + offset;
}

fVec2 Collider::GetEdgePosition(int index) {
	return GetPosition() + this->edges[index];
}


bool Collider::Sat(Collider* other, fVec2* depthBuffer) {
	float depth = INFINITY;
	for (int i = 0; i < this->edges.size() - 1; ++i) {
		fVec2 side   = GetEdgePosition((i + 1) % edges.size()) - GetEdgePosition(i);
		fVec2 normal = fVec2(side.y, -side.x).normalize();
		float minn = 0.0f;	 float maxx  = 0.0f;
		float min0 = INFINITY;  float max0 = -INFINITY;
	
		for (int j = 0; j < this->edges.size(); ++j) { //TODO::Improve this loop
			if (j == i || j == ((i + 1) % edges.size()))
				continue;
			fVec2 vec = GetEdgePosition(j) - GetEdgePosition(i);
			float proj = vec.Dot(normal);
		
			if (proj <= minn)
				minn = proj;
			
			if (proj >= maxx)
				maxx = proj;
		}

		for (int j = 0; j < other->edges.size(); ++j) {

			fVec2 vec = other->GetEdgePosition(j) - GetEdgePosition(i);
			float proj = vec.Dot(normal);

			if (proj <= min0)
				min0 = proj;

			if (proj >= max0)
				max0 = proj;
		}
		float temp = Min<float>(maxx - min0, max0 - minn);
		if ( temp <= depth) {
			depth = temp;
			*depthBuffer = normal * depth;		
		}
		if ( temp <= 0.0 ) {
			return false;
		}
	}
	return true;
}

bool Collider::isColliding(Collider* other, fVec2* depthBuffer) {
	fVec2 b0;
	fVec2 b1;
	fVec2 temp;

	if (depthBuffer == nullptr)
		depthBuffer = &temp;
	fVec2 dir = (this->GetPosition() - other->GetPosition()).normalize();

	if (!other->Sat(this, &b1) || !Sat(other, &b0))
		return false;

	if (b0.magnitude() < b1.magnitude())
		*depthBuffer = b0 * Sign<float>(dir.Dot(b0));
	else
		*depthBuffer = b1 * Sign<float>(dir.Dot(b1));
	return true;
}