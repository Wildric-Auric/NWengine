#include "Physics.h"
#include "Globals.h"
#include "Components.h"
//Rigidbody::Rigidbody(GameObject* attached) {
//	attachedGameObject = attached;
//}
//iVec2 Rigidbody::GetPosition() {
//	return attachedGameObject->GetComponent<Transform>().position;
//};
//void Rigidbody::Update() {
//	attachedGameObject->GetComponent<Transform>().position = attachedGameObject->GetComponent<Transform>().position + grav*deltaTime;
//};









//class point {
//public:
//	Vector2<int> position = Vector2<int>();
//	Vector2<int> previous = Vector2<int>();
//	bool lock;
//	GameObjectClone obj;
//};
//
//class stick {
//public:
//	point* pointA;
//	point* pointB;
//	float length;
//};
//const int m = 10;
//point points[m] = { point() };
//stick sticks[m - 1];
//
//
//Vector2<int> grav = Vector2<int>(0, -40);
//bool once = true;
//
//void Simulate(int n) {
//	points[0].lock = 1;
//
//	for (int i = 0; i < m; i++) {
//
//		points[0].position.x = -ORIGINAL_WIDTH / 2 + mousePosX;
//		points[0].position.y = +ORIGINAL_HEIGHT / 2 - mousePosY;
//
//
//		if (!points[i].lock) {
//			//Vector2<int> vel = points[i].position + points[i].previous*(-1);
//			//points[i].previous = points[i].position;
//			//points[i].positon = points[i].position + vel;
//			//points[i].position = points[i].position + grav;
//
//			Vector2<int> pos = points[i].position;
//			//points[i].position = points[i].position +(points[i].position + points[i].previous * (-1) );
//			points[i].position.y += grav.y;
//			points[i].previous = pos;
//
//		}
//
//	}
//
//
//	for (int i = 0; i < n;i++) {
//		for (stick s : sticks) {
//			Vector2<int> center = (s.pointA->position + s.pointB->position) * .5f;
//			Vector2<float> dir = (s.pointA->position * (-1) + (s.pointB->position)).normalize();
//			//float distance = (s.pointB->position + s.pointA->position * (-1)).magnitude();
//			iVec2 vec = iVec2(dir.x * 10, dir.y * 10);
//			//d1 = dir.x;
//			//d2 = dir.y;
//			if (once) {
//				if (!s.pointB->lock) {
//					s.pointB->position = center + vec;
//				}
//				if (!s.pointA->lock) {
//					s.pointA->position = center + vec * (-1);
//				}
//			}
//			else {
//				s.pointB->position.x = -ORIGINAL_WIDTH / 2 + mousePosX;
//				s.pointB->position.y = +ORIGINAL_HEIGHT / 2 - mousePosY;
//			}
//		}
//	}
//}