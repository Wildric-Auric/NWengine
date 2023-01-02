#include "Collision.h"
#include "Maths.h"
#include "Components.h"

Collider2::Collider2(GameObject* go) {
	this->attachedObj = go;
};


std::map<GameObject*, Collider2> Collider2::componentList;

void Collider2::SetEdges(std::vector<iVec2> newEdges) {
	edges.swap(newEdges);
	for (int i = 0; i < edges.size(); i++) {
		if (this->edges[i].magnitude() > this->edges[this->outerCircleEdge].magnitude()) this->outerCircleEdge = i;
	}
}


void Collider2::Resize(Vector2<int> newSize) {
	manualSize = newSize;
	scale = &manualSize;
};

Vector2<int> Collider2::GetPosition() {
	if (this != nullptr)
		return (*position) + offset;
	return Vector2<int>(0, 0);
};
Vector2<int> Collider2::GetSize() {
	if (this != nullptr) {
		fVec2 a = attachedObj->GetComponent<Transform>()->scale;
		iVec2 b = attachedObj->GetComponent<Sprite>()->texture->size;  
		return iVec2(abs(a.x * b.x), abs(a.y * b.y));
	}
	return iVec2(0, 0);
}


bool Collider2::isColliding(Collider2 other) {
	float radius = this->edges[this->outerCircleEdge].magnitude() + other.edges[this->outerCircleEdge].magnitude();
	if ( (*this->position  - *other.position).magnitude() > radius) return 0;

	int thisSize =  sizeof(this->edges) / sizeof(Vector2<int>);
	int otherSize = sizeof(other.edges) / sizeof(Vector2<int>);

	for (int i = 0; i < thisSize; i++) {
		for (int j = 0; i <= otherSize; j++) {
			iVec2 vec1 = this->edges[(i+1) % thisSize] - this->edges[i];
			iVec2 vec2 = this->edges[i] + *this->position - other.edges[j] - *other.position;
			int det = Det2(vec1, vec2);
			if (det < 0) return 1;
		}
	};

	for (int i = 0; i < otherSize; i++) {
		for (int j = 0; i < thisSize; j++) {
			iVec2 vec1 = other.edges[(i + 1) % otherSize] - other.edges[i];
			iVec2 vec2 = other.edges[i] + *other.position - this->edges[j] - *this->position;
			int det = Det2(vec1, vec2);
			if (det < 0) return 1;
		}
	};


	return 0;


}


void Collider2::Gui() {
	static int a = 3;
	NWGui::DragValue<int>("Fuck_unity123", &a, ImGuiDataType_S32);
}